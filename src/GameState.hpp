/*
 *  Octovasion - an RC Game Jam game!
 *  Copyright (C) 2017 Gwendolyn Weston, Jason Owen, Xianny Ng
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GameState_hpp
#define GameState_hpp

#include <vector>

struct Point {
    int x;
    int y;
};
bool operator==(const Point& lhs, const Point& rhs);

struct Rect {
    int x,
        y,
        width,
        height;

};
bool operator==(const Rect& lhs, const Rect& rhs);

enum class Action {
    NEW_GAME,
    LEFT,
    CENTER,
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
    Rect paddle;
    bool gameInProgress;

    std::vector<Point> bullets;

    GameState(int width, int height);
    void handleAction(Action action);
    int escapeBoundary();

private:
    void fireBullet(Point origin);
    void updatePaddleForEscapeBoundaryChange();
};



#endif /* GameState_hpp */
