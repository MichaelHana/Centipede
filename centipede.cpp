#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <vector>
#include <string>

using namespace sf;

int unit = 30;
RectangleShape initSpaceship(int windowX, int windowY)
{
    RectangleShape spaceship;
    spaceship.setSize(Vector2f(unit, unit));
    spaceship.setOrigin(spaceship.getSize().x/2, spaceship.getSize().y/2);
    spaceship.setPosition(windowX/2, windowY-spaceship.getSize().y - 10);
    return spaceship;
}

RectangleShape initProjectile(int width , int height)
{
    RectangleShape p;
    p.setSize(Vector2f(width, height));
    p.setOrigin(width/2, height/2);
    p.setFillColor(Color::Red);
    return p;
}

RectangleShape initStartButton(int windowX, int windowY)
{
    RectangleShape startButton;
    startButton.setSize(Vector2f(200, 100));
    startButton.setOrigin(startButton.getSize().x/2, startButton.getSize().y/2);
    startButton.setPosition(windowX/2, windowY/2);
    startButton.setFillColor(Color::Red);
    return startButton;
}

Text initStartButtonText(Vector2f pos)
{
    Text startText;
    startText.setCharacterSize(40);
    startText.setOrigin(55, 25);
    startText.setPosition(pos);
    startText.setString("START");
    startText.setFillColor(Color::Black);
    return startText;
}

std::vector<RectangleShape> createCentipede(int x)
{
    std::vector<RectangleShape> centipede;
    int centipedeSize = 10;
    int startY = -unit;

    RectangleShape head;
    head.setSize(Vector2f(unit, unit));
    head.setPosition(x, 0);
    centipede.push_back(head);

    for (int i = 1; i < centipedeSize; i++)
    {
        RectangleShape body;
        body.setSize(Vector2f(unit, unit));
        body.setPosition(x, i * startY);
        centipede.push_back(body);
    }

    return centipede;
}

std::list<RectangleShape> initMushrooms(int windowWidth , int windowHeight)
{
    std::list<RectangleShape> mushrooms;

    for (int i = 0; i < 100; i++)
    {
        RectangleShape m;
        int x = (rand() % (windowWidth/unit)) * unit;
        int y = (rand() % ((windowHeight - unit)/unit)) * unit;

        m.setPosition(x, y);
        m.setSize(Vector2f(unit, unit));

        mushrooms.push_back(m);
    }

    return mushrooms;
}

std::vector<RectangleShape> moveCentipede(std::vector<RectangleShape> centipede, bool * direction,
                                          std::list<RectangleShape> mushrooms, int winx, int winy)
{
    Vector2f target(centipede.begin()->getPosition());
    std::list<RectangleShape>::iterator it;

    for (int i = 0; i < centipede.size(); i++)
    {
        Vector2f nextTarget(centipede[i].getPosition());
        if (i == 0)
        {
            if (centipede[i].getPosition().y <= 0)
            {
                centipede[i].move(0, 30);
            }
            else
            {
                if (*direction == true)
                {
                    centipede[i].move(30, 0);
                    for (it = mushrooms.begin(); it != mushrooms.end(); it++)
                    {
                        if (it->getGlobalBounds().intersects(centipede[i].getGlobalBounds()))
                        {
                            centipede[i].move(-30, 30);
                            *direction = false;
                        }
                    }

                    if (centipede[i].getPosition().x+centipede[i].getSize().x >= winx)
                    {
                        centipede[i].move(-30, 30);
                        *direction = false;
                    }
                }
                else
                {
                    centipede[i].move(-30, 0);
                    for (it = mushrooms.begin(); it != mushrooms.end(); it++)
                    {
                        if (it->getGlobalBounds().intersects(centipede[i].getGlobalBounds()))
                        {
                            centipede[i].move(30, 30);
                            *direction = true;
                        }
                    }

                    if (centipede[i].getPosition().x <= 0)
                    {
                        centipede[i].move(30, 30);
                        *direction = true;
                    }
                }
            }
        }
        else
        {
           centipede[i].setPosition(target);
        }
        target = nextTarget;
    }

    return centipede;
}

int main()
{
    RenderWindow window(VideoMode(1000, 1000), "Centipede");

    //Importing textures
    Texture spaceshipTexture;
    spaceshipTexture.loadFromFile("textures/spaceship.png");
    Texture centipedeHead;
    centipedeHead.loadFromFile("textures/centipede-head.png");
    Texture centipedeBody;
    centipedeBody.loadFromFile("textures/centipede-body.png");
    Texture fullMushroom;
    fullMushroom.loadFromFile("textures/full-mushroom.png");
    Texture halfMushroom;
    halfMushroom.loadFromFile("textures/half-mushroom.png");
    Texture quartMushroom;
    quartMushroom.loadFromFile("textures/quarter-mushroom.png");

    //Initializing Spaceship
    RectangleShape spaceship = initSpaceship(window.getSize().x, window.getSize().y);
    spaceship.setTexture(&spaceshipTexture);

    //Initializing Projectile
    RectangleShape projectile = initProjectile(4, spaceship.getSize().y/2);
    projectile.setPosition(spaceship.getPosition());

    //Initializing Mushrooms
    std::list<RectangleShape> mushrooms = initMushrooms(window.getSize().x, window.getSize().y - spaceship.getSize().y);
    std::list<RectangleShape>::iterator mIT;
    for (mIT = mushrooms.begin(); mIT != mushrooms.end(); mIT++)
    {
        mIT->setTexture(&fullMushroom);
    }

    //Initializing Centipedes
    std::vector<std::vector<RectangleShape>> centipedes;
    std::vector<bool> centipedesD;
    centipedes.push_back(createCentipede(510));
    centipedesD.push_back(true); // true = right, false = left

    for (int i = 0; i < centipedes[0].size(); i++)
    {
        if (i == 0)
        {
            centipedes[0][i].setTexture(&centipedeHead);
        }
        else
        {
            centipedes[0][i].setTexture(&centipedeBody);
        }
    }

    int tf = rand() % 1;
    if (tf == 1)
    {
        centipedesD.push_back(true);
    }
    else
    {
        centipedesD.push_back(false);
    }

    //Start Button
    RectangleShape startButton = initStartButton(window.getSize().x, window.getSize().y);
    Text startText = initStartButtonText(startButton.getPosition());
    Font pixel;
    pixel.loadFromFile("pixel.ttf");
    startText.setFont(pixel);

    //Score Display
    Text scoreText;
    scoreText.setFont(pixel);
    scoreText.setCharacterSize(40);
    scoreText.setString("0");
    scoreText.setFillColor(Color::Red);
    scoreText.setPosition(0, 0);

    bool startScreen = true;
    bool gameScreen = false;
    bool isProjFired;
    int score = 0;
    int moveSpeed = 1;
    Clock moveCD;
    Clock spaceCD;

    while (window.isOpen())
    {
        //window event
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();

        if (gameScreen == true)
        {
            //Keyboard Input
            if (Keyboard::isKeyPressed(Keyboard::Right) &&
                spaceship.getPosition().x+spaceship.getSize().x/2+1 <= window.getSize().x)
            {
                spaceship.move(moveSpeed, 0);
                for (mIT = mushrooms.begin(); mIT != mushrooms.end(); mIT++)
                {
                    if (spaceship.getGlobalBounds().intersects(mIT->getGlobalBounds()))
                    {
                        spaceship.move(-moveSpeed, 0);
                    }
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Left) &&
                spaceship.getPosition().x-spaceship.getSize().x/2-1 >= 0)
            {
                spaceship.move(-moveSpeed, 0);
                for (mIT = mushrooms.begin(); mIT != mushrooms.end(); mIT++)
                {
                    if (spaceship.getGlobalBounds().intersects(mIT->getGlobalBounds()))
                    {
                        spaceship.move(moveSpeed, 0);
                    }
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Up) &&
                spaceship.getPosition().y-spaceship.getSize().y-1 >= 0)
            {
                spaceship.move(0, -moveSpeed);
                for (mIT = mushrooms.begin(); mIT != mushrooms.end(); mIT++)
                {
                    if (spaceship.getGlobalBounds().intersects(mIT->getGlobalBounds()))
                    {
                        spaceship.move(0, moveSpeed);
                    }
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) &&
                spaceship.getPosition().y+spaceship.getSize().y+1 <= window.getSize().y)
            {
                spaceship.move(0, moveSpeed);
                for (mIT = mushrooms.begin(); mIT != mushrooms.end(); mIT++)
                {
                    if (spaceship.getGlobalBounds().intersects(mIT->getGlobalBounds()))
                    {
                        spaceship.move(0, -moveSpeed);
                    }
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Space) && isProjFired == false)
            {
                isProjFired = true;
            }

            //Projectile
            if (isProjFired == true)
            {
                projectile.move(0, -2);
                window.draw(projectile);
                if (projectile.getPosition().y <= 0)
                {
                    isProjFired = false;
                    projectile.setPosition(spaceship.getPosition());
                }
            }
            else
            {
                projectile.setPosition(spaceship.getPosition());
            }

            //Mushrooms
            for (mIT = mushrooms.begin(); mIT != mushrooms.end(); mIT++)
            {
                if (projectile.getGlobalBounds().intersects(mIT->getGlobalBounds()))
                {
                    isProjFired = false;
                    projectile.setPosition(spaceship.getPosition());

                    if (mIT->getTexture() == &fullMushroom)
                    {
                        mIT->setTexture(&halfMushroom);
                    }
                    else if (mIT->getTexture() == &halfMushroom)
                    {
                        mIT->setTexture(&quartMushroom);
                    }
                    else if (mIT->getTexture() == &quartMushroom)
                    {
                        mushrooms.erase(mIT);
                        break;
                    }
                }
            }

            //Centipedes
            if (moveCD.getElapsedTime() >= seconds(0.1))
            {
                for (int i = 0; i < centipedes.size(); i++)
                {
                    bool d = centipedesD[i];
                    centipedes[i] = moveCentipede(centipedes[i], &d, mushrooms, window.getSize().x, window.getSize().y);
                    centipedesD[i] = d;
                }

                moveCD.restart();
            }

            for (int i = 0; i < centipedes.size(); i++)
            {
                for (int j = 0; j < centipedes[i].size(); j++)
                {
                    if (spaceship.getGlobalBounds().intersects(centipedes[i][j].getGlobalBounds()))
                    {
                        gameScreen = false;
                        startScreen = true;
                        spaceCD.restart();
                    }
                    else if (centipedes[i][j].getGlobalBounds().intersects(projectile.getGlobalBounds()))
                    {
                        isProjFired = false;
                        score++;
                        scoreText.setString(std::to_string(score));

                        RectangleShape mushroom;
                        mushroom.setTexture(&fullMushroom);
                        mushroom.setSize(Vector2f(unit, unit));
                        mushroom.setPosition(centipedes[i][j].getPosition());
                        mushrooms.push_back(mushroom);

                        if (j != 0 && j != centipedes[i].size()-1)
                        {
                            std::vector<RectangleShape> newCentipede;
                            for (int k = 1; k < centipedes[i].size() - j; k++)
                            {
                                newCentipede.push_back(centipedes[i][j+k]);
                                centipedes[i].erase(centipedes[i].begin()+j+k);
                            }
                            newCentipede[0].setTexture(&centipedeHead);
                            centipedes.push_back(newCentipede);
                            centipedes[i].erase(centipedes[i].begin()+j);
                        }

                        else if (j == 0)
                        {
                            if (centipedes[i].size() == 1)
                            {
                                centipedes.erase(centipedes.begin()+i);
                                centipedesD.erase(centipedesD.begin()+i);
                            }
                            else
                            {
                                centipedes[i][1].setTexture(&centipedeHead);
                                centipedes[i].erase(centipedes[i].begin() + j);
                            }
                        }

                        else if (j == centipedes[i].size() - 1)
                        {
                            centipedes[i].erase(centipedes[i].begin()+j);
                        }
                    }

                    else if (centipedes[i][j].getPosition().y+centipedes[i][j].getSize().y >= window.getSize().y)
                    {
                        gameScreen = false;
                        startScreen = true;
                        spaceCD.restart();
                    }
                }
            }

            if (centipedes.size() == 0 )
            {
                centipedes.push_back(createCentipede((rand() % (window.getSize().x/unit)) * unit));
                for (int i = 0; i < centipedes[0].size(); i++)
                {
                    if (i == 0)
                    {
                        centipedes[0][i].setTexture(&centipedeHead);
                    }
                    else
                    {
                        centipedes[0][i].setTexture(&centipedeBody);
                    }
                }

                tf = rand() % 1;
                if (tf == 1)
                {
                    centipedesD.push_back(true);
                }
                else
                {
                    centipedesD.push_back(false);
                }
            }
        }

        //Drawing Mushrooms
        for (mIT = mushrooms.begin(); mIT != mushrooms.end(); mIT++)
        {
            window.draw(*mIT);
        }

        //Drawing Centipedes
        for (int i = 0; i < centipedes.size(); i++)
        {
            for (int j = 0; j < centipedes[i].size(); j++)
            {
                window.draw(centipedes[i][j]);
            }
        }

        //Drawing Start
        if (startScreen == true)
        {
            if (Keyboard::isKeyPressed(Keyboard::Space) && spaceCD.getElapsedTime() >= seconds(1))
            {
                score = 0;
                spaceship.setPosition(window.getSize().x/2, window.getSize().y - spaceship.getSize().y);
                projectile.setPosition(spaceship.getPosition());
                isProjFired = false;
                scoreText.setString("0");

                centipedes.clear();
                centipedes.push_back(createCentipede(510));
                for (int i = 0; i < centipedes[0].size(); i++)
                {
                    if (i == 0)
                    {
                        centipedes[0][i].setTexture(&centipedeHead);
                    }
                    else
                    {
                        centipedes[0][i].setTexture(&centipedeBody);
                    }
                }

                centipedesD.clear();
                tf = rand() % 1;
                if (tf == 1)
                {
                    centipedesD.push_back(true);
                }
                else
                {
                    centipedesD.push_back(false);
                }

                mushrooms.clear();
                mushrooms = initMushrooms(window.getSize().x, window.getSize().y - spaceship.getSize().y);
                for (mIT = mushrooms.begin(); mIT != mushrooms.end(); mIT++)
                {
                    mIT->setTexture(&fullMushroom);
                }

                startScreen = false;
                gameScreen = true;
            }
            window.draw(startButton);
            window.draw(startText);
        }

        window.draw(spaceship);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
