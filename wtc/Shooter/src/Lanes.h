/**
 * Lane class. Implements a shooting gallery lane for 2 users.
 * Not thread safe and requires external synchronization to manipulate lanes.
 */

#ifndef LANES_H
#define LANES_H

#include <iostream>
#include <iomanip>
#include <ostream>
#include <assert.h>


/**
 * Thanks to Skurmedel for this code.
 */

enum Color
{
  white = 0,
  red,
  blue,
  violet
};

namespace Colors {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}


class Lanes
{
public:
  // Constructors
  Lanes(int nlanes);
  Lanes(const Lanes &from);
  ~Lanes();

  // Obtain color in specific lane
  Color Set(int index,Color c);

  // Set color in specific lane
  Color Get(int index);

  // Return number of lanes
  int Count();
  // print color of lane.
  void Print();
  //
  void Clear();

private:
  Color* lanes;
  int nlanes;
  /* data */
};



Lanes::Lanes(int nlanes) : nlanes(nlanes)
{
  lanes = new Color[nlanes];
}

Lanes::~Lanes()
{

  delete[] lanes;
}

Lanes::Lanes(const Lanes& from)
{
  nlanes = from.nlanes;
  lanes = new Color[nlanes];
  for (int i = 0; i < nlanes; ++i)
  {
    lanes[i] = from.lanes[i];
    /* code */
  }
}

Color Lanes::Set(int index, Color c)
{
  // You should be only trying to set red or blue.
  assert ((c == red) || (c == blue));

  // If violet you are already in trouble
  if (lanes[index] == violet) return violet;

  // If another color then a thread beat you. Figure out why you got here
  // in the first place. You shouldn't be trying to shoot a lane that has already
  // been shot by someone else OR shot by yourself earlier.
  if (lanes[index] != white)
  {
    Color OldColor = lanes[index];
    lanes[index] = (lanes[index] == c)? c : violet ;

    return OldColor;
  }

  // If I got here then I am sure my lane is white.

  lanes[index] = c;

  return white;

}

Color Lanes::Get(int index)
{

  return lanes[index];
}

int Lanes::Count()
{

  return nlanes;
}


void Lanes::Clear()
{
  for (int i = 0; i < nlanes; ++i)
  {
    lanes[i] = white;
  }
}


void Lanes::Print()
{
    Colors::Modifier reds(Colors::FG_RED);
    Colors::Modifier def(Colors::FG_DEFAULT);
    Colors::Modifier blues(Colors::FG_BLUE);
    Colors::Modifier greens(Colors::FG_GREEN);

    std::cout<<std::endl;

    for (int i = 0; i < nlanes; i++)
      std::cout << "|------";

    std::cout<<std::endl;
    std::cout << "|" ;



    for (int i = 0; i < nlanes; ++i)
    {


      switch (lanes[i])
      {
       case red: std::cout << reds << "##RD##";break;
       case blue: std::cout << blues << "##BL##";break;
       case violet:std::cout << greens << "##VI##";break;
       case white: std::cout << def << "##WH##";break;
       default: std::cout<<"Error";break;
      }
      std::cout << def << "|" ;
    }
    std::cout<<std::endl;
    for (int i = 0; i < nlanes; i++)
      std::cout << "|------";

    std::cout<<std::endl;


}



#endif
