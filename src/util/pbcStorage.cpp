/** @file pbcStorage.cpp
    This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2015 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

#include "pbcStorage.h"
#include "models/pbcPlaybook.h"
#include "util/pbcConfig.h"
#include "util/pbcExceptions.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <botan/botan.h>
#include <botan/pbkdf.h>
#include <botan/secmem.h>
#include <botan/data_src.h>
#include <QPrinter>
#include <QPainter>
#include "pbcVersion.h"

/**
 * @class PBCStorage
 * @brief PBCStorage is the responsible class for persistent storage of created
 * playbooks.
 *
 * This includes binary export and import in a self defined (encrypted) "*.pbc"
 * file format as well as graphical export of the playbooks in PDF file format.
 */

/**
 * @brief Checks whether the version of a loaded playbook is newer than the
 * application's version
 * @param version The version string of the loaded playbook
 */
void PBCStorage::checkVersion(const std::string &version) {
    // TODO(obr): do better version checking
    int result = PBCVersion::compareCurrentVersionTo(version);
    if(result < 0) {
        throw PBCStorageException("Cannot load playbook because it's created by a newer version of Playbook-Creator. Please download the latest version of Playbook-Creator!");  //NOLINT
    }
    assert(result >= 0);
    assert(BOTAN_VERSION_CODE >= BOTAN_VERSION_CODE_FOR(1, 10, 9));
}

/**
 * @brief Generates a cryptographic key from a password and a random salt value
 * and set them as the current key and salt
 * @param password The password that the key is derived from
 */
void PBCStorage::generateAndSetKey(const std::string &password) {
    Botan::AutoSeeded_RNG rng;
    Botan::SecureVector<Botan::byte> salt = rng.random_vec(_SALT_SIZE);
    boost::shared_ptr<Botan::PBKDF> pbkdf(Botan::get_pbkdf(_PBKDF));  // NOLINT
    Botan::OctetString key = pbkdf->derive_key(_KEY_SIZE, password,
                                               &salt[0], salt.size(),
                                               _PBKDF_ITERATIONS);
    setCryptoKey(key, salt);
}

/**
 * @brief Resets the current key and salt
 * @param key The new key
 * @param salt The new salt
 */
void PBCStorage::setCryptoKey(Botan::OctetString key,
                        Botan::SecureVector<Botan::byte> salt) {
    _saltSP.reset(new Botan::SecureVector<Botan::byte>(salt));
    _keySP.reset(new Botan::OctetString(key));
}

/**
 * @brief Encrypts the input string and writes the result to a file stream
 * @param input The input string (usually the serialized playbook)
 * @param outFile The output file
 */
void PBCStorage::encrypt(const std::string& input,
                         std::ofstream& outFile) {
    assert(_keySP != NULL && _saltSP != NULL);
    outFile.write((const char*)_saltSP->begin(), _saltSP->size());

    Botan::Pipe hasher(new Botan::Hash_Filter(_HASH, _HASH_SIZE));

    Botan::Pipe encryptor(Botan::get_cipher(_CIPHER,
                                            *_keySP,
                                            Botan::ENCRYPTION),
                          new Botan::DataSink_Stream(outFile));

    hasher.process_msg(input);
    Botan::SecureVector<Botan::byte> hash = hasher.read_all(0);
    outFile.write((const char*)hash.begin(), hash.size());
    encryptor.process_msg(input);
}

/**
 * @brief Decrypts the given file using a password and writes the result to
 * an  output stream
 * @param password The password string, which the decryption key is derived from
 * @param ostream The output stream to which the decrypted playbook is written
 * @param inFile The file where the encrypted playbook is stored
 */
void PBCStorage::decrypt(const std::string &password,
                         std::ostream &ostream,
                         std::ifstream &inFile) {
    boost::shared_ptr<Botan::PBKDF> pbkdf(Botan::get_pbkdf(_PBKDF));
    Botan::SecureVector<Botan::byte> salt(_SALT_SIZE);
    inFile.read(reinterpret_cast<char*>(&salt[0]), _SALT_SIZE);

    Botan::SecureVector<Botan::byte> hash(_HASH_SIZE);
    inFile.read(reinterpret_cast<char*>(&hash[0]), _HASH_SIZE);

    Botan::OctetString key = pbkdf->derive_key(_KEY_SIZE, password,
                                               &salt[0], salt.size(),
                                               _PBKDF_ITERATIONS);

    Botan::Pipe decryptor(Botan::get_cipher(_CIPHER, key, Botan::DECRYPTION),
                          new Botan::Fork(
                              new Botan::Hash_Filter(_HASH, _HASH_SIZE),
                              new Botan::DataSink_Stream(ostream)));

    Botan::DataSource_Stream source(inFile);
    decryptor.process_msg(source);
    Botan::SecureVector<Botan::byte> calculatedHash = decryptor.read_all(0);

    if(calculatedHash != hash) {
        throw PBCDecryptionException("wrong password?");
    } else {
        setCryptoKey(key, salt);
    }
}

/**
 * @brief Initializes the PBCStorage instance when a new playbook loaded. Hence
 * the key and the salt have to be reset.
 * @param fileName The file name of the new playbook
 */
void PBCStorage::init(const std::string &fileName) {
    _currentPlaybookFileName = fileName;
    _keySP.reset();
    _saltSP.reset();
}

/**
 * @brief Wrapper function which generates key and salt and sets the file name
 * first and the actual saving algorithm is implemented in writeToCurrentPlaybookFile()
 * @param password
 * @param fileName
 */
void PBCStorage::savePlaybook(const std::string& password,
                              const std::string& fileName) {
    generateAndSetKey(password);
    _currentPlaybookFileName = fileName;
    writeToCurrentPlaybookFile();
}

/**
 * @brief Checks if key and salt are set before saving the playbook, so we don't
 * need to enter the password again.
 */
void PBCStorage::automaticSavePlaybook() {
    if(_keySP != NULL && _saltSP != NULL) {
        writeToCurrentPlaybookFile();
    } else {
        throw PBCStorageException("Cannot save playbook automatically whitout key. The playbook must be saved manually (invoke savePlaybook()-method) first");  //NOLINT
    }
}

/**
 * @brief Writes the playbook to a string buffer using Boost serialization
 * framework.
 *
 * Encrypting and writing to file is done via PBCStorage::encrypt() function
 */
void PBCStorage::writeToCurrentPlaybookFile() {
    assert(_currentPlaybookFileName != "");
    std::string extension = _currentPlaybookFileName.substr(_currentPlaybookFileName.size() - 4);  //NOLINT
    assert(extension == ".pbc");
    std::stringbuf buff;
    std::ostream ostream(&buff);
    ostream << "Playbook-Creator" << "\n";
    ostream << "playbook" << "\n";
    ostream << PBCVersion::getVersionString() << "\n";
    boost::archive::text_oarchive archive(ostream);
    archive << *PBCPlaybook::getInstance();

    std::ofstream ofstream(_currentPlaybookFileName,
                           std::ios_base::out | std::ios_base::binary);

    try {
        encrypt(buff.str(), ofstream);
    } catch(std::exception& e) {
        ofstream.close();
        // std::remove(fileName.c_str());
        std::cout << e.what() << std::endl;  // TODO(obr): message to user
    }

    ofstream.close();
}

/**
 * @brief Passes a file to the PBCStorage::decrypt() function to decrypt it and
 * write it to a string buffer. The buffer is then read and deserialized to a
 * PBCPlaybook instance using Boost serialization framework
 * @param password The decryption password
 * @param fileName The path to the file where the playbook ist stored
 */
void PBCStorage::loadPlaybook(const std::string &password,
                              const std::string &fileName) {
    std::string extension = fileName.substr(fileName.size() - 4);
    assert(extension == ".pbc");
    std::stringbuf buff;
    std::ostream ostream(&buff);
    std::ifstream ifstream(fileName);

    try {
        decrypt(password,
            ostream,
            ifstream);
    } catch(std::exception& e) {
        throw PBCStorageException(e.what());  // TODD(obr): message to user
    }
    _currentPlaybookFileName = fileName;

    std::istream istream(&buff);

    int kBuffSize = 100;
    char buffer[kBuffSize];

    istream.getline(buffer, kBuffSize);
    std::string pbcString(buffer);
    assert(pbcString == "Playbook-Creator");

    istream.getline(buffer, kBuffSize);
    std::string pbString(buffer);
    assert(pbString == "playbook");

    istream.getline(buffer, kBuffSize);
    std::string version(buffer);
    checkVersion(version);

    boost::archive::text_iarchive archive(istream);
    archive >> *PBCPlaybook::getInstance();
}

/**
 * @brief Graphically exports the playbook in PDF file format.
 *
 * The plays are arranged in a grid.
 * @param fileName The PDF file to which the playbook is exported
 * @param playViews A list of PBCPlayViews in which the exported plays are
 * graphically rendered using the Qt framework
 * @param paperWidth The width of one page
 * @param paperHeight The height of one page
 * @param columns The number of columns on one page
 * @param rows The number of rows on one page
 * @param marginLeft The left margin
 * @param marginRight The right margin
 * @param marginTop The margin from the top
 * @param marginBottom The margin from the bottom
 */
void PBCStorage::exportAsPDF(const std::string& fileName,
                             std::list<boost::shared_ptr<PBCPlayView>> playViews,  //NOLINT
                             const unsigned int paperWidth,
                             const unsigned int paperHeight,
                             const unsigned int columns,
                             const unsigned int rows,
                             const unsigned int marginLeft,
                             const unsigned int marginRight,
                             const unsigned int marginTop,
                             const unsigned int marginBottom) {
    std::string extension = fileName.substr(fileName.size() - 4);
    assert(extension == ".pdf");
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(QString::fromStdString(fileName));
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QSizeF(paperWidth, paperHeight), QPrinter::Millimeter);

    printer.setPageMargins(marginLeft,
                           marginTop,
                           marginRight,
                           marginBottom,
                           QPrinter::Millimeter);
    assert(columns > 0);
    assert(rows > 0);

    boost::shared_ptr<qreal> pixelMarginLeftSP(new qreal());
    boost::shared_ptr<qreal> pixelMarginRightSP(new qreal());
    boost::shared_ptr<qreal> pixelMarginTopSP(new qreal());
    boost::shared_ptr<qreal> pixelMarginBottomtSP(new qreal());
    printer.getPageMargins(pixelMarginLeftSP.get(),
                           pixelMarginTopSP.get(),
                           pixelMarginRightSP.get(),
                           pixelMarginBottomtSP.get(),
                           QPrinter::DevicePixel);
    QSize playSize = printer.pageRect().size();
    playSize.setWidth(playSize.width() / columns);
    playSize.setHeight(playSize.height() / rows);

    printer.setPageMargins(0.0, 0.0, 0.0, 0.0, QPrinter::Millimeter);
    QPainter painter(&printer);
    bool paintBorder = true;
    if(marginLeft == 0 &&
       marginRight == 0 &&
       marginTop == 0 &&
       marginBottom == 0) {
        paintBorder = false;
    }
    painter.setPen(QPen(QBrush(Qt::red), 20, Qt::DashLine));
    QRectF borderRect(10,
                      10,
                      printer.paperRect().width() - 10,
                      printer.paperRect().height() - 10);
    if(paintBorder == true) {
        painter.drawRect(printer.paperRect());
    }


    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int columnCount = 1;
    unsigned int rowCount = 1;
    for(boost::shared_ptr<PBCPlayView> playViewSP : playViews) {
        playViewSP->render(&painter,
                           QRectF(QPointF(x + *pixelMarginLeftSP, y + *pixelMarginTopSP), playSize),  //NOLINT
                           QRectF(),
                           Qt::IgnoreAspectRatio);
        ++columnCount;
        x = x + playSize.width();
        if(columnCount > columns) {
            x = 0;
            y = y + playSize.height();
            ++rowCount;
            columnCount = 1;
        }
        if(rowCount > rows) {
            bool successful = printer.newPage();
            assert(successful == true);
            if(paintBorder == true) {
                painter.drawRect(borderRect);
            }
            rowCount = 1;
            y = 0;
        }
    }
}
