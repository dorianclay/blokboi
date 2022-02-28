#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
  protected:
    int _location[2];

    GameObject()
    {
    }

  public:
    void update(int x, int y)
    {
        _location[0] = x;
        _location[1] = y;
    }
    const int *location() const
    {
        return _location;
    }
};

#endif /* GAME_OBJECT_H */