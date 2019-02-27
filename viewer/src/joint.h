#ifndef _JOINT_H_
#define _JOINT_H_

#include <string>
#include <QtGui/QMatrix4x4>
#include <QVector4D>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "TriMesh.h"

class AnimCurve {
public :
	AnimCurve() {};
	~AnimCurve() {
		_values.clear();
	}
public :
	std::string name;					// name of dof
	std::vector<double> _values;		// different keyframes = animation curve
};


enum RotateOrder {roXYZ=0, roYZX, roZXY, roXZY, roYXZ, roZYX};

class Joint {
public :
	// the convention will be : joint_vertices are the points corresponding to the
	// joint [curr_joint -> child_joint], at first it is computed from offsets
	// then it is computed iteratively using the anim_curve keyframe data
	static std::vector<trimesh::point> joint_vertices;		// list of point corresponding to the joint
	static int total_joint_number;

	std::string _name;					// name of joint


	// to obtain the current position of the join
	// once the position_transform has been computed properly
	// one maybe use position_transform*(0.0, 0.0, 0.0, 1.0)
	// supposing a scale of 1.0 and an initial position for the root Join
	// to be (before any transform or offset), at (0.0, 0.0, 0.0)
	// then for each new frame we update the rXYZ and tXYZ values in each 
	// animCurve, we recompute the transform accordingly (compose of rotation + translation for the root, only rotation for the other joints)
	// then we can retrieve the current position at this particular frame and draw it
	// cf documentation default constructor of QMatrix4x4 is an identity matrix
	QMatrix4x4 rotate_transform;  // rotate up to now
	QMatrix4x4 translate_transform; // translate up to this point (w/ offset + keyframe of hip)
	// QMatrix4x4 position_transform;				// position in 3d space, updated each frame
	QVector4D current_position; // current position of the joint, updated each frame
	QMatrix4x4 current_transform;

	double _posX = 0.0; // global position
	double _posY = 0.0; // global position
	double _posZ = 0.0; // global position

	double _offX;						// initial offset in X
	double _offY;						// initial offset in Y
	double _offZ;						// initial offset in Z
	std::vector<AnimCurve> _dofs;				// keyframes : _animCurves[i][f] = i-th dof at frame f;
	double _curTx;						// current value of translation on X
	double _curTy;						// current value of translation on Y
	double _curTz;						// current value of translation on Z
	double _curRx;						// current value of rotation about X (deg)
	double _curRy;						// current value of rotation about Y (deg)
	double _curRz;						// current value of rotation about Z (deg)
	int _rorder;						// order of euler angles to reconstruct rotation
	Joint *parent = NULL;
	std::vector<Joint*> _children;	// children of the current joint


public :
	// Constructor :
	Joint() {};
	// Destructor :
	~Joint() {
		_dofs.clear();
		_children.clear();
	}

	// Create from data :
	static Joint* create(std::string name, double offX, double offY, double offZ, Joint* parent) {
		Joint::total_joint_number += 1;
		Joint* child = new Joint();
		child->_name = name;
		child->_offX = offX;
		child->_offY = offY;
		child->_offZ = offZ;
		child->_curTx = 0;
		child->_curTy = 0;
		child->_curTz = 0;
		child->_curRx = 0;
		child->_curRy = 0;
		child->_curRz = 0;
		child->parent = parent;
		if(parent != NULL) { // make sure parent has the newly create join in its children list
			parent->_children.push_back(child);
		}
		return child;
	}

	// compute the box around the joint
	// to debug we could only make use of lines
	void compute_joint_vertices(bool use_keyframe, int frame_id);

	// update the joint state
	void update_joint_state();
	
	// translate, update the transform 
	void set_translate_transform(double tx, double ty, double tz);


	//trimesh::point get_3d_position();
	void initPosition();

	// rotate
	void set_rotate_transform(double yaw, double pitch, double law);

	// Load from file (.bvh) :	
	static Joint* createFromFile(std::string fileName);

	void animate(int iframe=0);

	// Analysis of degrees of freedom :
	void nbDofs();
};


#endif
