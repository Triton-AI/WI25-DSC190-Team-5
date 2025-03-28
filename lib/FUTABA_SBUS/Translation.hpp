#ifndef TRANSLATION_HPP
#define TRANSLATION_HPP

enum TriSwitchMode {
    DOWN = 0,
    MID = 1,
    UP = 2
};

class Translation {
public:
    double normalize(int analogValue);
    TriSwitchMode getTriSwitchMode(int TriVal);
};

double Translation::normalize(int analogValue) {
    if (analogValue > 1800)
        analogValue = 1800;
    if (analogValue < 174)
        analogValue = 174;
    analogValue -= 992;
    return analogValue >= 0 ? static_cast<double>(analogValue) / (1800 - 992)
                            : static_cast<double>(analogValue) / (992 - 174);
}

TriSwitchMode Translation::getTriSwitchMode(int TriVal) {
    double norm = normalize(TriVal);
    if (norm < -0.5)
        return DOWN;
    else if (norm > 0.4)
        return UP;
    else
        return MID;
}

#endif // TRANSLATION_HPP
