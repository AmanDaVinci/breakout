/*
*
*	Breakout - The classic arcade game in C using Stanford Portable Library
*
*/

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 700
#define WIDTH 450

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// Speed of ball
#define SPEED 5

// lives
#define LIVES 3

// paddle
#define PADWIDTH 60
#define PADHEIGHT 10

// brick
#define BRICKX 40
#define BRICKY 10

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);
    
    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);
    add(window, ball);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);
    add(window, paddle);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    add(window, label);
    
    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // Velocity of ball initially
    double xvelocity = drand48() * SPEED;
    double yvelocity = -2.0;

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {                   
        // Bounce Ball
        move(ball, xvelocity, yvelocity);
        if (getX(ball) + 2 * RADIUS >= getWidth(window))
        {
            xvelocity = -xvelocity;
        }
        else if (getY(ball) <= 0)
        {
            yvelocity = -yvelocity;
        }
        else if (getX(ball) <= 0)
        {
            xvelocity = -xvelocity;
        }
        pause(7);
            
        // Dead
        if (getY(ball) + 2 * RADIUS >= getHeight(window))
        {
            lives--;
            setVisible(ball, false);    
            // Respawn
            waitForClick();
            setLocation(ball,WIDTH / 2 - RADIUS, HEIGHT / 2 - RADIUS);
            setVisible(ball, true);
            yvelocity = -yvelocity;
                                            
        }  
           
        // Paddle Movement
        GEvent mouse = getNextEvent(MOUSE_EVENT);
        if (mouse != NULL)
        {
            if (getEventType(mouse) == MOUSE_MOVED)
            {
                double x = getX(mouse) - PADWIDTH / 2;
                double y = HEIGHT - 5 * PADHEIGHT;
                setLocation(paddle, x, y);   
            }
        }
            
        // Collision
        GObject object = detectCollision(window, ball);
        if (object != NULL)
        {
            if (object == paddle)
            {
                yvelocity = -yvelocity; 
            }
                           
            else if (strcmp(getType(object), "GRect") == 0)
            {
                if (object != paddle)
                {
                    yvelocity = -yvelocity;
                    removeGWindow(window, object);
                    points++;
                    bricks--;
                    // Score
                    updateScoreboard(window, label, points);
                }
            }
         }
                               
    }              
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    double x = 5;
    double y = 40;
    string color = "BLUE";
   
    for (int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(x, y, BRICKX, BRICKY);
            setColor(brick, color);
            setFilled(brick, true);
            add(window, brick);           
            x = x + (BRICKX + 5);
        }
        x = 5;
        y = y + (BRICKY + 5);
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(WIDTH / 2 - RADIUS, HEIGHT / 2 - RADIUS, 2 * RADIUS, 2 * RADIUS);
    setColor(ball, "GREEN");
    setFilled(ball, true); 
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH / 2 - PADWIDTH / 2, HEIGHT - 5 * PADHEIGHT, PADWIDTH, PADHEIGHT);
    setColor(paddle, "DARK_GRAY");
    setFilled(paddle, true);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-100");
    setColor(label, "Light_Gray");
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2 + 40;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
