#ifndef PBCCONFIG_H
#define PBCCONFIG_H

#include "pbcSingleton.h"
#include "pbcDeclarations.h"
#include "models/pbcColor.h"
#include <string>

class PBCConfig : public PBCSingleton<PBCConfig> {
friend class PBCSingleton<PBCConfig>;

 private:
    bool _initialized;
    unsigned int _minWidth;
    unsigned int _minHeight;
    PBCColor _losColor;
    unsigned int _losWidth;
    double _losYFactor;
    PBCColor _fiveYdColor;
    unsigned int _fiveYdWidth;
    double _fiveYdYFactor;
    PBCColor _ballColor;
    double _ballWidthYd;
    double _playerWidthYd;


    unsigned int _canvasWidth;
    unsigned int _canvasHeight;


 protected:
    PBCConfig() :
       _initialized(false),
       _minWidth(600),
       _minHeight(300),
       _losColor(128U, 128U, 128U),
       _losWidth(5),
       _losYFactor(0.5),
       _fiveYdColor(128U, 128U, 128U),
       _fiveYdWidth(3),
       _fiveYdYFactor(0.25),
       _ballColor(139U, 69U, 19U),
       _ballWidthYd(1),
       _playerWidthYd(1.5),
       _canvasWidth(200),
       _canvasHeight(200) {
       // TODO(obr): initialize from File
    }

 public:
    void setCanvasSize(unsigned int canvasWidth, unsigned int canvasHeight) {
        _initialized = true;
        _canvasWidth = canvasWidth;
        _canvasHeight = canvasHeight;
    }

    unsigned int canvasWidth() {
        assert(_initialized == true);
        return _canvasWidth;
    }

    unsigned int canvasHeight() {
        assert(_initialized == true);
        return _canvasHeight;
    }

    PBCColor losColor() {
        return _losColor;
    }

    double losWidth() {
        return _losWidth;
    }

    unsigned int minWidth() {
        return _minWidth;
    }

    unsigned int minHeight() {
        return _minHeight;
    }

    unsigned int losY() {
        return _losYFactor * _canvasHeight;
    }

    PBCColor fiveYdColor() {
        return _fiveYdColor;
    }

    unsigned int fiveYdWidth() {
        return _fiveYdWidth;
    }

    unsigned int fiveYdY() {
        return _fiveYdYFactor * _canvasHeight;
    }


    PBCColor ballColor() {
        return _ballColor;
    }

    unsigned int ballWidth() {
        return _ballWidthYd * ydInPixel();
    }

    double ydInPixel() {
        return (_losYFactor - _fiveYdYFactor) * _canvasHeight / 5.0;
    }

    unsigned int playerWidth() {
        return _playerWidthYd * ydInPixel();
    }
};


#endif  // PBCCONFIG_H

