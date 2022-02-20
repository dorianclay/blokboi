#ifndef LOCATION_H
#define LOCATION_H

class Location
{
protected:
    int _current[2];
    Location();

public:
    Location(int x, int y);

    const int* location() const;

    void move(int x, int y);

    bool operator==(const Location& rhs) const;
    bool operator!=(const Location& rhs) const;
};

#endif /* LOCATION_H */