//
//  main.cpp
//  zombie.cpp
//
//  Created by Madeleine Murphy on 12/1/18.
//  Copyright © 2018 Madeleine Murphy. All rights reserved.
//

// zombies.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;



///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;              // max number of rows in the arena
const int MAXCOLS = 30;              // max number of columns in the arena
const int MAXZOMBIES = 150;          // max number of zombies allowed

const int UP      = 0;
const int DOWN    = 1;
const int LEFT    = 2;
const int RIGHT   = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
// type name, since it's mentioned in the Zombie declaration.

class Zombie
{
public:
    // Constructor
    Zombie(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool getAttacked(int dir);
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_hasBeenAttacked; //helper
    // You'll probably find that a zombie object needs an additional
    // data member to support your implementation of the behavior affected
    // by being clubbed and being injured.
};

class Player
{
public:
    // Constructor
    Player(Arena *ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;
    
    // Mutators
    void   stand();
    void   moveOrAttack(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_age;
    bool   m_dead;
};

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     zombieCount() const;
    int     numZombiesAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    
    // Mutators
    bool   addZombie(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackZombieAt(int r, int c, int dir);
    bool   moveZombies();
    
private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Zombie* m_zombies[MAXZOMBIES];
    int     m_nZombies;
  
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
};


///////////////////////////////////////////////////////////////////////////
//  Zombie implementation
///////////////////////////////////////////////////////////////////////////

Zombie::Zombie(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A zombie must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Zombie created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_hasBeenAttacked = false;
   
}

int Zombie::row() const
{
    return m_row;
}

int Zombie::col() const
{
    // Return what column the zombie is at.
   
    return m_col;  // This implementation compiles, but is incorrect.
}

void Zombie::move()
{
    // Attempt to move in a random direction; if we can't move, don't move
    int dir = randInt(0, NUMDIRS-1);  // dir is now UP, DOWN, LEFT, or RIGHT
    
    // Done:  Attempt to move in direction dir; if we can't move, don't move.
    int r = m_row;
    int c = m_col;
 
    if (m_arena->determineNewPosition(r, c, dir))   //move if it can
    {
        m_row = r;
        m_col = c;
    }
    
}

bool Zombie::getAttacked(int dir)  // return true if dies
{
    //  If the zombie has been attacked once before, return true
    // (since a second attack destroys a zombie).  Otherwise, if possible,
    // move the zombie in one position in direction dir and return false
    // (since it survived the injury).  Otherwise, do not move, but return
    // true (since the momentum from the blow would bump the zombie against
    // the wall, dealing it an additional fatal injury).
    int r = m_row;
    int c = m_col;
    
    
    if (m_hasBeenAttacked){         //if it's already been attacked, it's dead!
        
        return true;
    }
    else{
        if(m_arena->determineNewPosition(r, c, dir))
        {
            m_row = r;                      //move it in direction of the blow from player
            m_col = c;
            m_hasBeenAttacked = true;       //set this so we know it's been attacked
            return false;
        }
        else{                               //if it can't move, it's hit a wall and dies anyway
            m_hasBeenAttacked = true;
            return true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    // Return what row the player is at.
    
    return m_row;
}

int Player::col() const
{
    // Return what column the player is at.
    
    return m_col;
}

int Player::age() const
{
    // Return the player's age.
   
    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::moveOrAttack(int dir)
{
    m_age++;
    // If there is a zombie adjacent to the player in the direction
    // dir, attack it.  Otherwise, move the player to that position if
    // possible (i.e., if the move would not be off the edge of the arena).
    
    int rplayer = m_row;
    int cplayer = m_col;
    
    
   
    if (m_arena->determineNewPosition(rplayer, cplayer, dir))   //if new position is valid
    {
        if (m_arena->numZombiesAt(rplayer, cplayer) > 0)    //if theres a zombie, attack
            m_arena->attackZombieAt(rplayer, cplayer, dir);
        else{       //otherwise move
            m_row = rplayer;
            m_col = cplayer;
        }
    }
}

bool Player::isDead() const
{
    //  Return whether the player is dead.
    if (m_dead)
        return true;
    return false;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementations
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nZombies = 0;
}

Arena::~Arena()
{
    //Delete the player and all remaining dynamically allocated zombies.
   // cerr << "Entering Arena destructor" << endl;
    delete m_player;
    for(int k = 0; k < m_nZombies; k++)
        delete m_zombies[k];
   // cerr << "Leaving Arena destructor" << endl;
}

int Arena::rows() const
{
    // Return the number of rows in the arena.
    // Delete the following line and replace it with the correct code.
    return m_rows;
}

int Arena::cols() const
{
    //  Return the number of columns in the arena.
    // Delete the following line and replace it with the correct code.
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::zombieCount() const
{
    return m_nZombies;
}

int Arena::numZombiesAt(int r, int c) const
{
    // Return the number of zombies at row r, column c.

    int count = 0;
    for (int k = 0; k < m_nZombies; k++)
    {
        if (m_zombies[k]->row() == r && m_zombies[k]->col() == c)
            count++;
    }
    return count;
}

bool Arena::determineNewPosition(int& r, int& c, int dir) const
{
    // If a move from row r, column c, one step in direction dir
    // would go off the edge of the arena, leave r and c unchanged and
    // return false.  Otherwise, set r or c so that row r, column c, is
    // now the new position resulting from the proposed move, and
    // return true.
    switch (dir)
    {
        case UP:
            // Implement the behavior if dir is UP.
            if (r - 1 <= 0){
                return false;
            }
            else{
                r = r - 1;
                c = c;
                return true;
            }
            break;
        case DOWN:
            if (r + 1 > m_rows){
                return false;
            }
            else{
                r = r + 1;
                c = c;
                return true;
            }
            break;
        case LEFT:
            if (c - 1 <= 0)
                return false;
            else {
                r = r;
                c = c - 1;
                return true;
            }
            break;
        case RIGHT:
            if (c + 1 > m_cols)
                return false;
            else {
                r = r;
                c = c + 1;
                return true;
            }
            break;
        default:
            return false;
    }
    return true;
}

void Arena::display() const
{
    // Position (row,col) in the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';
    
    // Indicate each zombie's position
    //        If one zombie is at some grid point, set the char to 'Z'.
    //        If it's 2 though 8, set it to '2' through '8'.
    //        For 9 or more, set it to '9'.
   for (r = 1; r < rows() + 1; r++){
        for (c = 1; c < cols() + 1; c++){
            if (numZombiesAt(r, c) == 1)
                grid[r-1][c-1] = 'Z';
            else if (numZombiesAt(r, c) >= 2 && numZombiesAt(r, c) <= 8)
                grid[r-1][c-1] = '0' + numZombiesAt(r, c);
            else if (numZombiesAt(r, c) >= 9)
                grid[r-1][c-1] = '9';
        }
    }
    // Indicate player's position
    if (m_player != nullptr)
    {
        // Set the char to '@', unless there's also a zombie there,
        // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }
    
    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
    
    // Write message, zombie, and player info
    cout << endl;
    cout << "There are " << zombieCount() << " zombies remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addZombie(int r, int c)
{
    // If there are MAXZOMBIES zombies, return false.  Otherwise,
    // dynamically allocate a new Zombie at coordinates (r,c).  Save the
    // pointer to the newly allocated Zombie and return true.
    
    // Your function must work as specified in the preceding paragraph even
    // in this scenario (which won't occur in this game):  MAXZOMBIES
    // are added, then some are destroyed, then more are added.
    
    
    //  Implement this.
 
    if (!(r > 0  &&  r <= m_rows  &&  c > 0  &&  c <= m_cols))   //if we are out of bounds, don't add a zombie
        return false;
    
    //if we're at a spot with a player, don't add zombie
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
    {
        return false;
    }
    
    if (m_nZombies >= MAXZOMBIES)
        return false;
    else
    {
        m_zombies[m_nZombies] = new Zombie(this, r, c);
        m_nZombies++;
        return true;
    }
    return false;
}

bool Arena::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

bool Arena::attackZombieAt(int r, int c, int dir)
{
    //  Attack one zombie at row r, column c if at least one is at
    // that position.  If the zombie does not survive the injury, destroy the
    // zombie object, removing it from the arena, and return true.  Otherwise,
    // return false (no zombie at (r,c), or zombie didn't die).
    
    if (numZombiesAt(r, c) < 1)
        return false;
    else{
        for (int k = 0; k < m_nZombies; k++){
            if (m_zombies[k]->row() == r && m_zombies[k]->col() == c){  //find the zombie at that row
                if(m_zombies[k]->getAttacked(dir))                      //if it's been attacked twice
                {
                    delete m_zombies[k];                                //delete that zombie
                    for (int j = k; j < m_nZombies - 1; j++)
                        m_zombies[j] = m_zombies[j+1];                  //shift all of the zombies over one memory slot
                    m_nZombies--;                                       //reduce count of zombies
                   
                    return true;
                }
                break;                                                  //once we find one zombie at that position, stop?
            }
        }
    }
        
    return false;
}

bool Arena::moveZombies()
{
    for (int k = 0; k < m_nZombies; k++)
    {
        m_zombies[k]->move();
        int r = m_zombies[k]->row();
        int c = m_zombies[k]->col();
        if (!m_player->isDead()){
            if ((r == m_player->row()) && (c == m_player->col()))
                m_player->setDead();
            }
    }
        //  Have the k-th zombie in the arena make one move.
        //        If that move results in that zombie being in the same
        //        position as the player, the player dies.
    
    
    // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nZombies)
{
    if (nZombies < 0)
    {
        cout << "***** Cannot create Game with negative number of zombies!" << endl;
        exit(1);
    }
    if (nZombies > MAXZOMBIES)
    {
        cout << "***** Trying to create Game with " << nZombies
        << " zombies; only " << MAXZOMBIES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nZombies > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the zombies!" << endl;
        exit(1);
    }
    
    // Create arena
    m_arena = new Arena(rows, cols);
    
    // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_arena->addPlayer(rPlayer, cPlayer);
    
    // Populate with zombies
    while (nZombies > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        // Don't put a zombie where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_arena->addZombie(r, c);
        nZombies--;
    }
}

Game::~Game()
{
    delete m_arena;
}

void Game::play()
{
    m_arena->display();
    Player* p = m_arena->player();
    if (p == nullptr)
        return;
    while ( ! m_arena->player()->isDead()  &&  m_arena->zombieCount() > 0)
    {
        cout << endl;
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)  // player stands
            p->stand();
        else
        {
            switch (action[0])
            {
                default:   // if bad move, nobody moves
                    cout << '\a' << endl;  // beep
                    continue;
                case 'q':
                    return;
                case 'u':
                case 'd':
                case 'l':
                case 'r':
                    p->moveOrAttack(decodeDirection(action[0]));
                    break;
            }
        }
        m_arena->moveZombies();
        m_arena->display();
    }
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
        case 'u':  return UP;
        case 'd':  return DOWN;
        case 'l':  return LEFT;
        case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

// Return a random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2); orginal Game g(7, 8, 25)
    Game g(7, 8, 25);
    
    // Play the game
    g.play();
}



///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                               &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif

