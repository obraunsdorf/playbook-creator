#include "mainDialog.h"
#include "ui_mainDialog.h"

#include "models/pbcPlay.h"


MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(printPlayNames()));
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::printPlayNames() {
    PBCPlaySP play1(new PBCPlay("Scissors", "sci"));
    PBCPlaySP play2(new PBCPlay("Big Tasty", "bt"));
    std::string printString = play1->getName() + ", " + play2->getName();
    ui->plainTextEdit->setPlainText(QString::fromStdString(printString));
}
