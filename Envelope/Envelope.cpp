/*
 * Envelope.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: Thrifleganger
 */
#include "Envelope.h"

// Function definitions for SingleSegment Class
SingleSegment::SingleSegment(double start, double dur, double end, bool hold /*=true*/, bool repeat /*=false*/){
	startPos = start;
	duration = dur * getSrate();
	endPos = end;
	count = 0;
	currentPos = startPos;
	increment = 0;
	this->hold = hold;
	this->repeat = repeat;
}

// Function definitions for Lineseg Class
void Lineseg::generate() {
	for(unsigned int i = 0; i < getVectorSize(); i++){
		vector[i] = currentPos;
		if(count < duration){
			currentPos += increment;
			count++;
		} else{
			if(repeat)
				Lineseg::retrigger();
			else{
				if(hold)
					currentPos = endPos;
				else
					currentPos += increment;
					count++;
			}

		}
	}
}

void Lineseg::retrigger() {
	count = 0;
	currentPos = startPos;
	increment = (endPos - startPos) / duration;
}

Lineseg::Lineseg(double start, double dur, double end, bool hold /*=true*/, bool repeat /*=false*/) :
	SingleSegment(start, dur, end, hold, repeat) {
	increment = (endPos - startPos) / duration;
}

//Function definitions for Expseg Class
void Expseg::generate() {
	for(unsigned int i = 0; i < getVectorSize(); i++){
		vector[i] = currentPos;
		if(count < duration){
			currentPos *= increment;
			count++;
		} else{
			if(repeat)
				Expseg::retrigger();
			else{
				if(hold)
					currentPos = endPos;
				else
					currentPos *= increment;
					count++;
			}
		}
	}
}

void Expseg::retrigger() {
	count = 0;
	currentPos = startPos;
	increment = pow((endPos / startPos), 1/(double)duration);
}

Expseg::Expseg(double start, double dur, double end, bool hold /*=true*/, bool repeat /*=false*/) :
	SingleSegment(start, dur, end, hold, repeat) {
	if(start == 0.0)
		startPos = 0.001;
	if(end == 0.0)
		endPos = 0.001;
	currentPos = startPos;
	increment = pow((endPos / startPos), 1/(double)duration);
}

//Function definitions for Multisegment Class
MultiSegment::MultiSegment(std::vector<double> position, std::vector<double> timeVect, bool hold, bool repeat){
	this->position = position;
	this->time = timeVect;
	for(unsigned int i = 0; i < this->time.size(); i++)
		this->time[i] *= getSrate();
	count = 0;
	currentPos = position[0];
	increment = 0;
	currentStart = position[0];
	currentEnd = position[1];
	currentDuration = time[0];
	numPositions = position.size();
	numTimes = time.size();
	vectorCount = 0;
	this->hold = hold;
	this->repeat = repeat;
}

//Function definitions for Linesegs Class
void Linesegs::generate() {
	for(unsigned int i = 0; i < getVectorSize(); i++){
		vector[i] = currentPos;
		if(count < currentDuration){
			currentPos += increment;
			count++;
		} else {
			if (vectorCount < numTimes - 1) {
				recompute();
				currentPos += increment;
			}
			else {
				if(repeat)
					Linesegs::retrigger();
				else{
					if(hold)
						currentPos = currentEnd;
					else
						currentPos += increment;
						count++;
				}
			}
		}
	}
}

void Linesegs::retrigger() {
	vectorCount = 0;
	count = 0;
	currentStart = position[0];
	currentEnd = position[1];
	currentDuration = time[0];
	currentPos = position[0];
	increment = (currentEnd - currentStart) / time[0];
}

void Linesegs::recompute() {
	currentStart = position[++vectorCount];
	currentEnd = position[vectorCount + 1];
	currentDuration += time[vectorCount];
	increment = (currentEnd - currentStart) / time[vectorCount];
}

Linesegs::Linesegs(std::vector<double> posVect, std::vector<double> timeVect, bool hold, bool repeat) :
	MultiSegment(posVect,timeVect,hold,repeat) {
	increment = (currentEnd - currentStart) / currentDuration;
}

//Function definitions for Expsegs Class
void Expsegs::generate() {
	for(unsigned int i = 0; i < getVectorSize(); i++){
		vector[i] = currentPos;
		if(count < currentDuration){
			currentPos *= increment;
			count++;
		} else {
			if (vectorCount < numTimes - 1) {
				recompute();
				currentPos *= increment;
			}
			else {
				if(repeat)
					Expsegs::retrigger();
				else {
					if(hold)
						currentPos = currentEnd;
					else
						currentPos *= increment;
						count++;
				}
			}
		}
	}
}

void Expsegs::retrigger() {
	vectorCount = 0;
	count = 0;
	currentStart = position[0];
	currentEnd = position[1];
	currentDuration = time[0];
	currentPos = position[0];
	increment = pow((currentEnd / currentStart), 1/time[vectorCount]);
}

void Expsegs::recompute() {
	currentStart = position[++vectorCount];
	currentEnd = position[vectorCount + 1];
	currentDuration += time[vectorCount];
	increment = pow((currentEnd / currentStart), 1/time[vectorCount]);
}

Expsegs::Expsegs(std::vector<double> posVect, std::vector<double> timeVect, bool hold, bool repeat) :
	MultiSegment(posVect,timeVect,hold,repeat) {
	for(unsigned int i = 0; i < position.size(); i++) {
		if(position[i] == 0)
			position[i] = 0.001;
	}
	currentPos = position[0];
	increment = pow((currentEnd / currentStart), 1/(double)currentDuration);
}
