/*
 * Rogue.h
 *
 *  Created on: 
 *      Author: Shi Yihui
 */
#ifndef ROGUE_H
#define ROGUE_H

extern int g_round;

class Rogue
{
public:
	Rogue(Color color, int rate, Lanes* gallery, int lane_num):
	    m_isCleaner(false),
	    m_hasCleaner(false),
		m_Bullet(color),
	    m_ShotRate(rate),
	    m_Gallery(gallery),
	    m_LaneNum(lane_num),
	    m_action_count(0),
	    m_succ_count(0),
	    m_lastRound(g_round){};
	virtual ~Rogue(){};

    bool IsGalleryFullPainted();

public:
	/* data */
    bool m_isCleaner;
    bool m_hasCleaner;
	Color m_Bullet; // The bullet color to paint the lane
	int m_ShotRate; // Rate/s required to shoot the lanes
	int m_Success; // Rate/s of lanes shot by ROGUE   
	Lanes* m_Gallery;
	int m_LaneNum;
	
	int m_action_count;
	int m_succ_count;
	int m_lastRound;
	
	void Run();
	virtual bool Shoot()=0;
	virtual void Clear()=0;
};

#endif
