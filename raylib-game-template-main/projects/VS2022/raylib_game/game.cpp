#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "Vehicle.cpp"

#include <iostream>

using namespace std;

struct AutoAgents
{
    Vector2 target;
    //Both agents
    Color luffyTint = ColorBrightness(BLUE, -1.0f);
    int fruitsEaten = 0;
    bool type1;
    float desiredAngle;
    float loseRangeTime;
    bool shouldDraw = true;
    float angle = 0;
    float brightness = -1.0f;
    bool isMutated;
    Vehicle luffy = Vehicle(rand() % 1551 + 50, rand() % 901 + 30);
    bool loseRangeCheck;

    //Simarbir's agent
    float circleRadius = 40.0f;
    float loseRangeThreshold = 3.0f;

    //Shrihith's agent
    int bigRadius = 60;
    int smallRadius = 40;

} agents[10];

struct shipStruct
{
    int foodEaten = 0;
    float angle = 0;
    float desiredAngle;
    Vector2 target;
    Vehicle ship = Vehicle(50, 50);
}ships[5];

struct DevilFruit
{
    int xPos, yPos, radius;
    Color tint;
} fruits[50];

int main(void)
{
    InitWindow(1600, 950, "raylib [core] example - basic window");
    SetTargetFPS(30);

    int oddsEvolving = 0;
    Image luffy = LoadImage("C:\\Users\\shrih\\OneDrive\\Desktop\\REALRAYLIB\\raylib-game-template-main\\projects\\VS2022\\raylib_game\\resources\\ship-removebg-preview.png");
    Texture2D luffyTexture = LoadTextureFromImage(luffy);
    UnloadImage(luffy);

    //Image ship = LoadImage("\"C:\\Users\\Simarbir G\\Documents\\raylib-project\\raylib-game-template-main\\projects\\VS2022\\raylib_game\\resources\\sunny-removebg-preview-removebg-preview.png\"");
    Image ship = LoadImage("resources/ship-removebg-preview.png");
    Texture2D shipTexture = LoadTextureFromImage(ship);
    UnloadImage(ship);

    for(int i = 0; i < 10; i++)
        if(i < 5)
            agents[i].type1 = true;
        else
            agents[i].type1 = false;

    float time = 10.0f;
    float changeLocTime = 3.0f;

    float evolutionMultiplier = 1.0f;

    int chosenRandomNumX;
    int chosenRandomNumY;
    int indexRemove = -1;

    bool ranOnce = false;

    while (!WindowShouldClose())
    {
        if(time >= 10.0f)
        {
            for(int i = 0; i < size(agents); i++)
            {
                for(int j = i+1; j < size(agents); j++)
                {
                    if(agents[i].fruitsEaten > agents[j].fruitsEaten)
                    {
                        AutoAgents temp = agents[i];
                        agents[i] = agents[j];
                        agents[j] = temp;
                    }
                }
            }

            for(int i = 0; i < size(agents); i++)
                cout << to_string(agents[i].fruitsEaten) + "\n";

            for(int i = 0; i < size(fruits); i++)
            {
                fruits[i].xPos = rand() % 1551 + 50;
                fruits[i].yPos = rand() % 901 + 30;
                fruits[i].radius = 10.0f;
                fruits[i].tint = PURPLE;
            }

            for(int i = 0; i < size(agents); i++)
            {
                chosenRandomNumX = rand() % 1551 + 50;
                chosenRandomNumY = rand() % 901 + 30;
                agents[i].target.x = (float)chosenRandomNumX;
                agents[i].target.y = (float)chosenRandomNumY;

                if(i == indexRemove && ranOnce)
                {
                    agents[i].shouldDraw = false;
                }

                agents[i].fruitsEaten = 0;
                agents[i].brightness = -1.0f;
                agents[i].luffyTint = ColorBrightness(BLUE, agents[i].brightness);
            }

            indexRemove++;
            if(ranOnce){
                for(int i = 0; i < size(agents); i++){
                    for(int j = 0; j < size(agents); j++){
                        if(!agents[i].type1 && agents[j].type1 && CheckCollisionCircles(Vector2({ agents[i].luffy.location.x, agents[i].luffy.location.y }), agents[i].bigRadius, Vector2({ agents[j].luffy.location.x, agents[j].luffy.location.x }), agents[j].circleRadius)){
                            agents[j].shouldDraw = false;
                            agents[i].isMutated = true;
                        }
                    }
                }
            }


            for(int i = size(agents) - 1; i >= 0; i--)
            {
                int randNum = rand() % 10 + 1;
                if(randNum > oddsEvolving && ranOnce)
                {
                    if(agents[i].type1){
                        agents[i].luffy.boostMaxSpeed(0.2f);
                        agents[i].loseRangeThreshold -= 0.3f;
                    }else{
                        agents[i].bigRadius += 20;
                    }

                }

                oddsEvolving++;
            }

            oddsEvolving = 0;
            time = 0;
        }

        BeginDrawing();

        time += GetFrameTime();
        changeLocTime += GetFrameTime();

        for(int i = 0; i < size(agents); i++)
        {
            // Check each time if type 1
            if(agents[i].loseRangeCheck)
                agents[i].loseRangeTime += GetFrameTime();
        }

        ClearBackground(WHITE);

        for(int i = 0; i < size(agents); i++)
        {
            agents[i].desiredAngle = atan2(agents[i].luffy.velocity.y, agents[i].luffy.velocity.x);
            agents[i].angle = Lerp(agents[i].angle, agents[i].desiredAngle, 0.1f);
        }

        //float desiredAngle = atan2(car.velocity.y, car.velocity.x);
        //angle = Lerp(angle, desiredAngle, 0.1f);

        //if(angle < -360)
        //    angle *= -1;

        //if(angle > 360)
        //    angle = (int)angle % 360;

        if(time < 10.0f)
        {
            ranOnce = true;

            for(int i = 0; i < 50; i++)
                DrawCircle(fruits[i].xPos, fruits[i].yPos, fruits[i].radius, fruits[i].tint);

            for(int i = 0; i < size(agents); i++)
            {
                if(agents[i].shouldDraw && agents[i].type1)
                {
                    DrawCircle((int)agents[i].luffy.location.x, (int)agents[i].luffy.location.y, agents[i].circleRadius, agents[i].luffyTint);
                    DrawTexturePro(luffyTexture, Rectangle({0,0,(float)(luffyTexture.width),(float)(luffyTexture.height)}),
                                   Rectangle({agents[i].luffy.location.x, agents[i].luffy.location.y, (float)(luffyTexture.width), (float)(luffyTexture.height)}),
                                   Vector2({(float)(luffyTexture.width / 2), (float)(luffyTexture.height / 2)}), (int)(agents[i].angle * (180 / PI)) % 360, WHITE);
                }else if(agents[i].shouldDraw && !agents[i].type1){
                    if(agents[i].isMutated){
                        DrawCircle((int)agents[i].luffy.location.x, (int)agents[i].luffy.location.y, agents[i].bigRadius, ORANGE);
                        //cout << "AAAA";
                    }else{
                        DrawCircle((int)agents[i].luffy.location.x, (int)agents[i].luffy.location.y, agents[i].bigRadius, GREEN);
                    }

                    DrawCircle((int)agents[i].luffy.location.x, (int)agents[i].luffy.location.y, agents[i].smallRadius, agents[i].luffyTint);
                    DrawTexturePro(luffyTexture, Rectangle({0,0,(float)(luffyTexture.width),(float)(luffyTexture.height)}),
                                   Rectangle({agents[i].luffy.location.x, agents[i].luffy.location.y, (float)(luffyTexture.width), (float)(luffyTexture.height)}),
                                   Vector2({(float)(luffyTexture.width / 2), (float)(luffyTexture.height / 2)}), (int)(agents[i].angle * (180 / PI)) % 360, WHITE);
                    agents[i].luffy.maxspeed = 1.5f;
                }
            }

            for(int i = 0; i < size(agents); i++)
            {
                agents[i].luffy.seek(agents[i].target);
                agents[i].luffy.update();
            }

            for(int i = 0; i < size(fruits); i++)
                for(int j = 0; j < size(agents); j++)
                {
                    if(agents[j].type1){
                        if(CheckCollisionCircles(Vector2({agents[j].luffy.location.x, agents[j].luffy.location.y}), 40, Vector2({(float)fruits[i].xPos, (float)fruits[i].yPos}), fruits[i].radius) && !agents[j].loseRangeCheck && agents[j].shouldDraw)
                        {
                            fruits[i].xPos = 3000;
                            agents[j].luffyTint = BLANK;
                            agents[j].brightness += 0.15f;
                            agents[j].fruitsEaten++;
                            agents[j].loseRangeCheck = true;
                        }
                    }else if(!agents[j].type1){
                        if (CheckCollisionCircles(Vector2({ agents[j].luffy.location.x, agents[j].luffy.location.y }), agents[j].bigRadius, Vector2({ (float)fruits[i].xPos, (float)fruits[i].yPos }), fruits[i].radius)) {
                            agents[j].target.x = (float)fruits[i].xPos;
                            agents[j].target.y = (float)fruits[i].yPos;
                            agents[j].luffy.seek(agents[j].target);
                            if (CheckCollisionCircles(Vector2({ agents[j].luffy.location.x, agents[j].luffy.location.y }), agents[j].smallRadius, Vector2({ (float)fruits[i].xPos, (float)fruits[i].yPos }), 10)) {
                                //dots[i].tint = BLANK;
                                fruits[i].xPos = 5000;
                                fruits[i].yPos = 5000;
                                agents[j].target.x = rand() % 1400;
                                agents[j].target.y = rand() % 750;
                                agents[j].brightness += 0.15f;
                                agents[j].fruitsEaten++;
                                if(agents[j].isMutated){
                                    agents[j].luffyTint = BLANK;
                                    agents[j].loseRangeCheck = true;
                                }
                            }
                        }
                    }
                }
        }

        if(changeLocTime >= 3.0f)
        {
            for(int i = 0; i < size(agents); i++)
            {
                chosenRandomNumX = rand() % 1551 + 50;
                chosenRandomNumY = rand() % 901 + 30;
                agents[i].target.x = (float)chosenRandomNumX;
                agents[i].target.y = (float)chosenRandomNumY;
            }

            changeLocTime = 0;
        }

        for(int i = 0; i < size(agents); i++)
        {
            // Check each time if type 1

            if(agents[i].loseRangeTime >= agents[i].loseRangeThreshold)
            {
                agents[i].luffyTint = ColorBrightness(BLUE, agents[i].brightness);
                agents[i].loseRangeCheck = false;
                agents[i].loseRangeTime = 0;
            }
        }

        EndDrawing();
    }

    CloseWindow();
}

