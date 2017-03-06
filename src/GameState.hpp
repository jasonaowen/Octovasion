//
//  GameState.hpp
//  GIP
//
//  Created by gwendolyn weston on 3/6/17.
//
//

#ifndef GameState_hpp
#define GameState_hpp

#include <vector>

struct Point {
    int x;
    int y;
};

bool operator==(const Point& lhs, const Point& rhs);

enum class Action {
    LEFT,
    RIGHT,
    FIRE_LEFT_BULLET,
    FIRE_RIGHT_BULLET,
    CHECK_PADDLE_COLLISION,
    ESCAPE_BULLET,
    MOVE_BULLET
};

class GameState {
public:
    const int worldWidth, worldHeight;
    const Point rightTentacle, leftTentacle;
    int capturedOctobabies;
    Point paddle;
    std::vector<Point> bullets;

    GameState(int width, int height);
    void handleAction(Action action);

private:
    void fireBullet(Point origin);
};



#endif /* GameState_hpp */