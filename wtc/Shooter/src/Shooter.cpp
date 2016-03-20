/*
 * Shooter.cpp
 *
 *  Created on: 
 *      Author: Shi Yihui
 */
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <ctime>
#include <thread>
#include "Lanes.h"
#include "Rogue.h"
#include <vector>
#include <immintrin.h>  /* for _mm_pause() */
#include "rtm.h"

const int LANE_NUM = 16;

Lanes* Gallery;
using namespace std;

static int g_ClassCategory = 2;
int g_round = 10;

static pthread_mutex_t CoarseMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t FineMutex[LANE_NUM];
static pthread_mutex_t CleanerMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static struct option long_options[] =
  {
    {"red rate", required_argument, 0, 'r'},
    {"blue rate", required_argument, 0, 'b'},
    {"round", required_argument, 0, 'n'},
    {"class", required_argument, 0, 'c'},
    {0, 0, 0, 0}
  };

bool Rogue::IsGalleryFullPainted()
{
    bool result = true;
    
    for(int i=0; i<m_LaneNum; i++)
    {
        if(m_Gallery->Get(i)==white)
        {
            result = false;
            break;
        }
    }
    return result;
}

void Rogue::Run()
{
    while(1)
    {
        if(m_hasCleaner&&m_isCleaner)
        {
            pthread_mutex_lock(&CleanerMutex); 
            pthread_cond_wait(&cond, &CleanerMutex);
            
            if(IsGalleryFullPainted())
            {
                Clear();
                g_round --;
            }
            pthread_mutex_unlock(&CleanerMutex); 

            if(g_round<=0)
                break;
            else 
                continue;
        }
    
        if(m_lastRound!=g_round)
        {
            m_lastRound = g_round;
            if(m_Bullet==red)
            {
                cout<<"Red success rate in this round:"<<(((double)m_succ_count)*100.0/((double)m_action_count))<<"%"<<endl;
                m_succ_count=0;
                m_action_count=0;
            }
        }
        if(g_round<=0)break;
        
        bool bingo = Shoot();
        if(bingo)
        {
            m_succ_count++;
        }
        m_action_count++;

        if(!m_hasCleaner)
        {
            if(IsGalleryFullPainted())
            {
                Clear();
                g_round --;
            }
        }
        else if(bingo)
        {
            pthread_cond_signal(&cond);
        }

        sleep(m_ShotRate);
    }

    if(m_Bullet==blue)
    {
        cout<<"Blue total success rate:"<<(((double)m_succ_count)*100.0/((double)m_action_count))<<"%"<<endl;
        m_succ_count=0;
        m_action_count=0;
    }
}

class RogueCoarse:public Rogue
{
public:
	RogueCoarse(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){};
	~RogueCoarse(){};

	bool Shoot();
	void Clear();
};

bool RogueCoarse::Shoot()
{
    int lane;
    bool result=false;

    lane = rand()%m_LaneNum;
    pthread_mutex_lock(&CoarseMutex);
    if(m_Gallery->Get(lane)==white)
    {
        m_Gallery->Set(lane,m_Bullet);
        result = true;
    }
    pthread_mutex_unlock(&CoarseMutex);

    return result;
}

void RogueCoarse::Clear()
{
    pthread_mutex_lock(&CoarseMutex);
    m_Gallery->Print();
    m_Gallery->Clear();
    pthread_mutex_unlock(&CoarseMutex);
}


class RogueFine:public Rogue
{
public:
	RogueFine(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){};
	~RogueFine(){};

	bool Shoot();
	void Clear();
};

bool RogueFine::Shoot()
{
    int lane;
    bool result=false;
    
    lane = rand()%m_LaneNum;
    pthread_mutex_lock(FineMutex+lane);
    if(m_Gallery->Get(lane)==white)
    {
        m_Gallery->Set(lane,m_Bullet);
        result = true;
    }
    pthread_mutex_unlock(FineMutex+lane);
    return result;
}

void RogueFine::Clear()
{
    for(int i=0; i<m_LaneNum; i++)
        pthread_mutex_lock(FineMutex+i);
    m_Gallery->Print();
    m_Gallery->Clear();
    for(int i=0; i<m_LaneNum; i++)
        pthread_mutex_unlock(FineMutex+i);
}

class RogueCoarse2:public Rogue
{
public:
	RogueCoarse2(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){};
	~RogueCoarse2(){};

	bool Shoot();
	void Clear();
};

bool RogueCoarse2::Shoot()
{
    int lane1,lane2;
    bool result=false;

    lane1 = rand()%m_LaneNum;
    while(1)
    {
        lane2 = rand()%m_LaneNum;
        if(lane1!=lane2)
            break;
    }
    pthread_mutex_lock(&CoarseMutex);
    if((m_Gallery->Get(lane1)==white)&&(m_Gallery->Get(lane2)==white))
    {
        m_Gallery->Set(lane1,m_Bullet);
        m_Gallery->Set(lane2,m_Bullet);
        result = true;
    }
    pthread_mutex_unlock(&CoarseMutex);

    return result;
}

void RogueCoarse2::Clear()
{
    pthread_mutex_lock(&CoarseMutex);
    m_Gallery->Print();
    m_Gallery->Clear();
    pthread_mutex_unlock(&CoarseMutex);
}

class RogueFine2:public Rogue
{
public:
	RogueFine2(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){};
	~RogueFine2(){};

	bool Shoot();
	void Clear();
};

bool RogueFine2::Shoot()
{
    int lane1,lane2;
    bool result = false;

    lane1 = rand()%m_LaneNum;
    while(1)
    {
        lane2 = rand()%m_LaneNum;
        if(lane1!=lane2)
            break;
    }
    if(lane1>lane2)
    {
        int tmp = lane1;
        lane1 = lane2;
        lane2 = tmp;
    }
    pthread_mutex_lock(FineMutex+lane1);
    pthread_mutex_lock(FineMutex+lane2);
    if((m_Gallery->Get(lane1)==white)&&(m_Gallery->Get(lane2)==white))
    {
        m_Gallery->Set(lane1,m_Bullet);
        m_Gallery->Set(lane2,m_Bullet);
        result = true;
    }
    pthread_mutex_unlock(FineMutex+lane2);
    pthread_mutex_unlock(FineMutex+lane1);
    
    return result;
}

void RogueFine2::Clear()
{
    for(int i=0; i<m_LaneNum; i++)
        pthread_mutex_lock(FineMutex+i);
    m_Gallery->Print();
    m_Gallery->Clear();
    for(int i=0; i<m_LaneNum; i++)
        pthread_mutex_unlock(FineMutex+i);
}

class RogueCoarseCleaner:public Rogue
{
public:
	RogueCoarseCleaner(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){m_hasCleaner=true;};
	RogueCoarseCleaner(Lanes* gallery, int lane_num)
	    :Rogue(white, 0, gallery, lane_num){m_isCleaner=true;m_hasCleaner=true;};
	~RogueCoarseCleaner(){};

	bool Shoot();
	void Clear();
};

bool RogueCoarseCleaner::Shoot()
{
    int lane;
    bool result = false;
    
    lane = rand()%m_LaneNum;
    
    pthread_mutex_lock(&CoarseMutex);
    if(m_Gallery->Get(lane)==white)
    {
        m_Gallery->Set(lane,m_Bullet);
        result = true;
    }
    pthread_mutex_unlock(&CoarseMutex);
    return result;
}

void RogueCoarseCleaner::Clear()
{
    pthread_mutex_lock(&CoarseMutex);
    m_Gallery->Print();
    m_Gallery->Clear();
    pthread_mutex_unlock(&CoarseMutex);
}

class RogueFineCleaner:public Rogue
{
public:
	RogueFineCleaner(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){m_hasCleaner=true;};
	RogueFineCleaner(Lanes* gallery, int lane_num)
	    :Rogue(white, 0, gallery, lane_num){m_isCleaner=true;m_hasCleaner=true;};
	~RogueFineCleaner(){};

	bool Shoot();
	void Clear();
};

bool RogueFineCleaner::Shoot()
{
    int lane;
    bool result = false;
    lane = rand()%m_LaneNum;
    
    pthread_mutex_lock(FineMutex+lane);
    if(m_Gallery->Get(lane)==white)
    {
        m_Gallery->Set(lane,m_Bullet);
        result = true;
    }
    pthread_mutex_unlock(FineMutex+lane);

    return result;
}

void RogueFineCleaner::Clear()
{
    for(int i=0; i<m_LaneNum; i++)
        pthread_mutex_lock(FineMutex+i);
    m_Gallery->Print();
    m_Gallery->Clear();
    for(int i=0; i<m_LaneNum; i++)
        pthread_mutex_unlock(FineMutex+i);
}


class RogueTM:public Rogue
{
public:
	RogueTM(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){};
	~RogueTM(){};

	bool Shoot();
	void Clear();
};

bool RogueTM::Shoot()
{
    int lane;
    bool result = false;
    
    lane = rand()%m_LaneNum;

    int status;
    int nretries=100;
    
    while(nretries>0)
    {
        if ((status = _xbegin ()) == _XBEGIN_STARTED) 
        {
            if(m_Gallery->Get(lane)==white)
            {
                m_Gallery->Set(lane,m_Bullet);
                result = true;
            }
            _xend ();
            break;
        }
        else {
            result = false;
            nretries--;
        }    
    }

    return result;
}

void RogueTM::Clear()
{
    int status;
    int nretries=100;

    while(nretries>0)
    {
        if ((status = _xbegin ()) == _XBEGIN_STARTED) 
        {

            m_Gallery->Print();
            m_Gallery->Clear();

            _xend ();
            break;
        }
        else {
            nretries--;
        }    
    }
}


class RogueTM2:public Rogue
{
public:
	RogueTM2(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){};
	~RogueTM2(){};

	bool Shoot();
	void Clear();
};

bool RogueTM2::Shoot()
{
    int lane1,lane2;
    bool result = false;

    lane1 = rand()%m_LaneNum;
    while(1)
    {
        lane2 = rand()%m_LaneNum;
        if(lane1!=lane2)
            break;
    }

    int status;
    int nretries=100;

    while(nretries>0)
    {
        if ((status = _xbegin ()) == _XBEGIN_STARTED) 
        {

            if((m_Gallery->Get(lane1)==white)&&(m_Gallery->Get(lane2)==white))
            {
                m_Gallery->Set(lane1,m_Bullet);
                m_Gallery->Set(lane2,m_Bullet);
                result = true;
            }

            _xend ();
            break;
        }
        else {
            result = false;
            nretries--;
        }    
    }

    return result;
}

void RogueTM2::Clear()
{
    int status;
    int nretries=100;

    while(nretries>0)
    {
        if ((status = _xbegin ()) == _XBEGIN_STARTED) 
        {

            m_Gallery->Print();
            m_Gallery->Clear();

            _xend ();
            break;
        }
        else {
            nretries--;
        }    
    }
}

class RogueTMCleaner:public Rogue
{
public:
	RogueTMCleaner(Color color, int rate, Lanes* gallery, int lane_num)
	    :Rogue(color, rate, gallery, lane_num){m_hasCleaner=true;};
	RogueTMCleaner(Lanes* gallery, int lane_num)
	    :Rogue(white, 0, gallery, lane_num){m_isCleaner=true;m_hasCleaner=true;};
	~RogueTMCleaner(){};

	bool Shoot();
	void Clear();
};

bool RogueTMCleaner::Shoot()
{
    int lane;
    bool result = false;
    lane = rand()%m_LaneNum;

    int status;
    int nretries=100;

    while(nretries>0)
    {
        if ((status = _xbegin ()) == _XBEGIN_STARTED) 
        {

            if(m_Gallery->Get(lane)==white)
            {
                m_Gallery->Set(lane,m_Bullet);
                result = true;
            }

            _xend ();
            break;
        }
        else {
            result = false;
            nretries--;
        }    
    }    

    return result;
}

void RogueTMCleaner::Clear()
{
    int status;
    int nretries=100;

    while(nretries>0)
    {
        if ((status = _xbegin ()) == _XBEGIN_STARTED) 
        {

            m_Gallery->Print();
            m_Gallery->Clear();

            _xend ();
            break;
        }
        else {
            nretries--;
        }    
    }
}

void Printer(int rate) {

    /**
     *  NOT TRANSACTION SAFE; cannot be called inside a transaction. Possible conflict with other Txs Performs I/O
     *  Print lanes periodically rate/s.
     *  If it doesn't synchronize then possible mutual inconsistency between adjacent lanes.
     *  Not a particular concern if we don't shoot two lanes at the same time.
     *
     */

   while(1)
   {
       sleep(1);
       if(g_round<=0)break;
       Gallery->Print();
       //cout<<Gallery->Count();
   }

}

void ShooterAction(int rate,Color PlayerColor);
void Cleaner();

extern char *optarg;

int main(int argc, char** argv)
{
    int red_speed = 2;
    int blue_speed = 2;
  
  // Code based on the example code in the getopt documentation
  while (true) {

    int option_index = 0;    
    int c = getopt_long_only(argc, argv, "r:b:n:c:",
                             long_options, &option_index);
    
    /* Detect the end of the options. */
    if (c == -1)
      break;
    
    switch (c) {
    case 0:
      /* If this option set a flag, do nothing else now. */
      break;

    case 'r':
      red_speed = atoi(optarg);
      break;
      
    case 'b':
      blue_speed = atoi(optarg);
      break;
      
    case 'n':
      g_round = atoi(optarg);
      break;
      
    case 'c':
      g_ClassCategory = atoi(optarg);
      break;

    case '?':
      /* getopt_long already printed an error message. */
      exit(1);
      break;
      
    default:
      exit(1);
    }
  }
    std::vector<thread> ths;

    Gallery = new Lanes(LANE_NUM);
    
    ths.push_back(std::thread(&ShooterAction, red_speed, red));
    ths.push_back(std::thread(&ShooterAction, blue_speed, blue));
    ths.push_back(std::thread(&Cleaner));
    ths.push_back(std::thread(&Printer,5));

    for (auto& th : ths) {
        th.join();
    }

    return 0;
}

void ShooterAction(int rate,Color PlayerColor) {

    /**
     *  Needs synchronization. Between Red and Blue shooters.
     *  Choose a random lane and shoot.
     *  Rate: Choose a random lane every 1/rate s.
     *  PlayerColor : Red/Blue.
     */
    srand(time(0));

    Rogue *rogue=0;
    switch(g_ClassCategory)
    {
    case 1:
        rogue = new RogueCoarse(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    case 2:
        rogue = new RogueCoarse2(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    case 3:
        rogue = new RogueCoarseCleaner(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    case 4:
        rogue = new RogueFine(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    case 5:
        rogue = new RogueFine2(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    case 6:
        rogue = new RogueFineCleaner(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    case 7:
        rogue = new RogueTM(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    case 8:
        rogue = new RogueTM2(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    default:
        rogue = new RogueTMCleaner(PlayerColor, rate, Gallery, LANE_NUM);
        break;
    }

    if(rogue!=0)
    {
        rogue->Run();
        delete(rogue);
    }
}

void Cleaner() {

    /**
     *  Cleans up lanes. Needs to synchronize with shooter.
     *  Use a monitor
     *  Should be in action only when all lanes are shot.
     *  Once all lanes are shot. Cleaner starts up.
     *  Once cleaner starts up shooters wait for cleaner to finish.
     */
    srand(time(0));  

    Rogue *rogue=0;
    switch(g_ClassCategory)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        rogue = new RogueCoarseCleaner(Gallery, LANE_NUM);
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        rogue = new RogueFineCleaner(Gallery, LANE_NUM);
        break;
    case 7:
        break;
    case 8:
        break;
    default:
        rogue = new RogueTMCleaner(Gallery, LANE_NUM);
        break;
    }

    if(rogue!=0)
    {
        rogue->Run();
        delete(rogue);
    }
}

