//
//  GameStateTests.cpp
//  GIP
//
//  Created by gwendolyn weston on 3/6/17.
//
//

#include "catch.hpp"
#include "../src/GameState.hpp"

TEST_CASE("moving left moves left") {
    GameState state = GameState(10, 10);
    Rect initialPaddle = state.paddle;

    state.handleAction(Action::LEFT);

    CHECK(initialPaddle.x - 1 == state.paddle.x);
}


TEST_CASE("moving past wall does not move left") {
    GameState state = GameState(10, 10);
    state.paddle.x = 0;

    state.handleAction(Action::LEFT);

    CHECK(state.paddle.x == 0);
}

TEST_CASE("moving right moves right") {
    GameState state = GameState(10, 10);
    Rect initialPaddle = state.paddle;

    state.handleAction(Action::RIGHT);

    CHECK(initialPaddle.x + 1 == state.paddle.x);
}


TEST_CASE("moving past wall does not move right") {
    GameState state = GameState(10, 10);
    state.paddle.x = state.worldWidth;

    state.handleAction(Action::RIGHT);

    CHECK(state.paddle.x == state.worldWidth);
}

TEST_CASE("fire right bullet fires a bullet from the right!!") {
    GameState state = GameState(10, 10);

    CHECK(state.bullets.size() == 0);

    state.handleAction(Action::FIRE_RIGHT_BULLET);

    REQUIRE(state.bullets.size() == 1);
    CHECK(state.bullets[0] == state.rightTentacle);
}

TEST_CASE("fire left bullet fires a bullet from the left!!") {
    GameState state = GameState(10, 10);

    CHECK(state.bullets.size() == 0);

    state.handleAction(Action::FIRE_LEFT_BULLET);

    REQUIRE(state.bullets.size() == 1);
    CHECK(state.bullets[0] == state.leftTentacle);
}

TEST_CASE("collision with no bullets does nothing") {
    GameState state = GameState(10, 10);

    state.handleAction(Action::CHECK_PADDLE_COLLISION);

    CHECK(state.bullets.size() == 0);
    CHECK(state.capturedOctobabies == 0);
}

TEST_CASE("check collision does nothing if bullet outside of paddle") {
    GameState state = GameState(10, 10);
    state.paddle.x = 9;
    state.bullets.push_back({ 8, 0 });

    state.handleAction(Action::CHECK_PADDLE_COLLISION);

    CHECK(state.bullets.size() == 1);
    CHECK(state.capturedOctobabies == 0);
}

TEST_CASE("check collision detects a bullet in the paddle") {
    GameState state = GameState(10, 10);
    state.paddle.x = 9;
    state.bullets.push_back({ 9, 0 });

    state.handleAction(Action::CHECK_PADDLE_COLLISION);

    CHECK(state.bullets.size() == 0);
    CHECK(state.capturedOctobabies == 1);
}

TEST_CASE("tiny world creates non-zero paddle") {
    GameState state = GameState(1, 1);

    CHECK(state.paddle.height > 0);
    CHECK(state.paddle.width > 0);
}
