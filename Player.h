//
//  Player.h
//  CS 32 Project 1
//
//  Created by Madeleine Murphy on 4/4/19.
//  Copyright © 2019 Madeleine Murphy. All rights reserved.
//

#ifndef Player_h
#define Player_h

class Arena;

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
#endif /* Player_h */
