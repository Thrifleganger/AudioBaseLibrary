/**
	Envelope.h

	Header file containing classes for generating single segment,
	multi segment and ADSR envelopes.

	Hierarchy:

		CLASS				DEPENDENCIES

	1)	SingleSegment		AudioParams
	2) 	Lineseg				AudioParams, SingleSegment
	3) 	Expseg				AudioParams, SingleSegment
	4) 	MultiSegment		AudioParams
	5)	Linesegs			AudioParams, MultiSegment
	6)	Expsegs				AudioParams, MultiSegment

	@Author Thrifleganger
	@Version 1.0
*/

#ifndef ENVELOPE_H_
#define ENVELOPE_H_

#include <cmath>
#include <iostream>
#include <vector>
#include "AudioBase.h"

/**
 * SingleSegment: Base class for all single segment envelopes
 */
class SingleSegment : public AudioBuffer{

protected:
	long duration;
	long count;
	double startPos;
	double endPos;
	double currentPos;
	double increment;
	bool hold;
	bool repeat;

	virtual void generate() = 0;

	virtual void retrigger() = 0;

	//virtual void checkInputValues();

public:

	SingleSegment(double start, double dur, double end, bool hold = true, bool repeat = false);

	virtual ~SingleSegment(){}

	/**
	 * Recursively generate envelope samples of the shape that was used while instantiating
	 * the envelope object. To be called recursively inside a processing loop.
	 *
	 * @param None
	 * @return A constant double pointer of size 'vectorSize', filled with interpolated
	 * envelope values
	 */
	virtual const AudioBuffer process(){
		generate();
		return *this;
	}

	virtual const AudioBuffer operator()() { return process(); }

	/**
	 * Resets the envelope pointer to start from the start of the shape that was used
	 * while instantiating the envelope object.
	 *
	 * @param None
	 * @return None
	 */
	virtual void reset(){
		retrigger();
	}

	/**
	 * Resets the envelope pointer to start from the start. New envelope values can be
	 * specified which overwrites the values specified while instantiating the
	 * envelope object
	 *
	 * @param start: Position or value at the start of the envelope.
	 * 			duration: Duration in seconds, between start and end.
	 * 			end: Position or value at the end of the envelope.
	 * 			hold(Def - true): Holds the value at 'end' after 'duration', if true. If
	 * 				false, continues interpolating based on the current shape.
	 * 			repeat(Def - false): Repeats the envelope upon reaching 'end'.
	 * @return None
	 */
	virtual void reset(double start, double duration, double end, bool hold = true, bool repeat = false){
		startPos = start;
		this->duration = duration;
		endPos = end;
		this->hold = hold;
		this->repeat = repeat;
		retrigger();
	}

};

/**
 *	Class for calling a 2 point straight line as envelope. The value of line segment end point
 *	can either be maintained after the envelope ends, or can be extented outward in the as an unbounded
 *	line. The envelope can also be repeated upon reaching the end point
 */
class Lineseg : public SingleSegment{

protected:
	void generate();
	void retrigger();

public:
	/**
	 * 	Constructor for instantiating a 2 point linear envelope.
	 *
	 *	@param start Position or value at the start of the envelope.
	 * 	@param duration Duration in seconds, between start and end.
	 * 	@param end Position or value at the end of the envelope.
	 * 	@param hold Holds the value at 'end' after 'duration', if true. If
	 * 				false, continues interpolating based on the current shape.
	 * 	@param repeat Repeats the envelope upon reaching 'end'.
	 * 	@return None
	 */
	Lineseg(double start, double dur, double end, bool hold = true, bool repeat = false);

};

/**
 *	Class for calling a 2 point exponential curve as envelope. The value of curve end point
 *	can either be maintained after the envelope ends, or can be extented outward in the as an unbounded
 *	curve shape. Care should be taken for exponential growth as the values can get unstable over time.
 *	The envelope can also be repeated upon reaching the end point
 */
class Expseg : public SingleSegment{

protected:
	void generate();
	void retrigger();

public:
	/**
	 * 	Constructor for instantiating a 2 point exponential envelope.
	 *
	 *	@param start Position or value at the start of the envelope.
	 * 			duration: Duration in seconds, between start and end.
	 * 			end: Position or value at the end of the envelope.
	 * 			hold(Def - true): Holds the value at 'end' after 'duration', if true. If
	 * 				false, continues interpolating based on the current shape.
	 * 			repeat(Def - false): Repeats the envelope upon reaching 'end'.
	 * 	@return None
	 */
	Expseg(double start, double dur, double end, bool hold = true, bool repeat = false);

};


/**
 * SingleSegment: Base class for all multi segment envelopes
 */
class MultiSegment : public AudioBuffer{

protected:
	long count;
	std::vector<double> position;
	std::vector<double> time;
	double currentPos;
	double increment;
	int currentDuration;
	double currentStart;
	double currentEnd;
	double numPositions;
	double numTimes;
	int vectorCount;
	bool hold;
	bool repeat;

	virtual void generate() = 0;

	virtual void retrigger() = 0;

	virtual void recompute() = 0;

public:
	MultiSegment(std::vector<double> position, std::vector<double> timeVect, bool hold = true, bool repeat = false);

	virtual ~MultiSegment(){
	}

	/**
	 * Recursively generate envelope samples of the shape that was used while instantiating
	 * the envelope object. To be called recursively inside a processing loop.
	 *
	 * @param None
	 * @return A constant double pointer of size 'vectorSize', filled with interpolated
	 * envelope values
	 */
	virtual const AudioBuffer process(){
		generate();
		return *this;
	}

	virtual const AudioBuffer operator()() { return process(); }

	/**
	 * Resets the envelope pointer to the default values provided in the vectors used
	 * while instantiating the envelope object.
	 *
	 * @param None
	 * @return None
	 */
	virtual void reset(){
		retrigger();
	}

	/**
	 * Resets the envelope pointer to start from the start. New envelope values can be
	 * specified which overwrites the values specified while instantiating the
	 * envelope object
	 *
	 * @param position Position or value vector of envelope points
	 * 			time: A vector of duration in seconds, between position points.
	 * 			hold(Def - true): Holds the value at 'end' after 'duration', if true. If
	 * 				false, continues interpolating based on the current shape.
	 * 			repeat(Def - false): Repeats the envelope upon reaching 'end'.
	 * @return None
	 */
	virtual void reset(std::vector<double> position, std::vector<double> timeVect, bool hold = true, bool repeat = false){
		this->position = position;
		this->time = timeVect;
		for(unsigned int i = 0; i < this->time.size(); i++)
			this->time[i] *= getSrate();
		this->hold = hold;
		this->repeat = repeat;
		retrigger();
	}
};

/**
 *	Class for calling multi point straight lines as envelope. The final value of line segment end point
 *	can either be maintained after the envelope ends, or can be extented outward in the as an unbounded
 *	line. The envelope can also be repeated upon reaching the end point
 */
class Linesegs: public MultiSegment{
protected:
	void generate();
	void retrigger();
	void recompute();

public:

	/**
	 * 	Constructor for instantiating a multi point linear envelope.
	 *
	 *	@param position: Position or value vector of envelope points
	 * 			time: A vector of duration in seconds, between position points.
	 * 			hold(Def - true): Holds the value of the last position, if true. If
	 * 				false, continues interpolating based on the current shape.
	 * 			repeat(Def - false): Repeats the envelope upon reaching last position.
	 * 	@return None
	 */
	Linesegs(std::vector<double> position, std::vector<double> timeVect, bool hold = true, bool repeat = false);
};

/**
 *	Class for calling multi point exponential curves as envelope. The final value of curve end point
 *	can either be maintained after the envelope ends, or can be extented outward in the as an unbounded
 *	curve. The envelope can also be repeated upon reaching the end point
 */
class Expsegs: public MultiSegment{
protected:
	void generate();
	void retrigger();
	void recompute();

public:

	/**
	 * 	Constructor for instantiating a multi point exponential envelope.
	 *
	 *	@param position: Position or value vector of envelope points
	 * 			time: A vector of duration in seconds, between position points.
	 * 			hold(Def - true): Holds the value of the last position, if true. If
	 * 				false, continues interpolating based on the current shape.
	 * 			repeat(Def - false): Repeats the envelope upon reaching last position.
	 * 	@return None
	 */
	Expsegs(std::vector<double> position, std::vector<double> timeVect, bool hold = true, bool repeat = false);
};

#endif /* ENVELOPE_H_ */
