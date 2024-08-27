#include "raylib.h"
#include "raymath.h"

#include <iostream>



class Vehicle {

public:

    Vector2 location;
    Vector2 velocity;
    Vector2 acceleration;
    float r;
    float maxforce;
    float maxspeed;

    Vehicle(float x, float y) {
        acceleration = Vector2({ 0, 0 });
        velocity = Vector2({ 0, 0 });
        location = Vector2({ x, y });
        r = 3.0;
        maxspeed = 3;
        maxforce = 0.15f;
    }

    void update() {
        velocity = Vector2Add(velocity, acceleration);
        velocity = Vector2ClampValue(velocity, 0.0f, maxspeed);
        location = Vector2Add(velocity, location);
        acceleration = Vector2Scale(acceleration, 0);
    }

    void applyForce(Vector2 force) {
        acceleration = Vector2Add(force, acceleration);
    }

    void seek(Vector2 target) {
        Vector2 desired = Vector2Subtract(target, location);
        desired = Vector2Normalize(desired);
        desired = Vector2Multiply(desired, Vector2({ maxspeed, maxspeed }));
        Vector2 steer = Vector2Subtract(desired, velocity);
        steer = Vector2ClampValue(steer, 0.0f, maxforce);
        applyForce(steer);
    }
    void boostMaxSpeed(float m)
    {
        maxspeed += m;
    }

};
