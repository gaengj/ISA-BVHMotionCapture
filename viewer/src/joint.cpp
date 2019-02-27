#include "joint.h"
#include <QtGui/QMatrix4x4>
#include <QTransform>
#include <list>
#include <string>
#include "TriMesh.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <QVector4D>

using namespace std;

std::vector<trimesh::point> Joint::joint_vertices;
int Joint::total_joint_number = 0;


// for skinning it should be as follow :
//	local -> for each joint
//	transform that are maintained allow from local to global
//	use transform to go from local to global
// 	skinning is weight the joints
// 	weights.txt gives corresponding points to joints + weights
//	use inverse transform to go from global to local
//	once in local do skinning stuff?

std::vector<std::string> rstripArray(const std::vector<std::string>& array)
{
	std::string tmp_string;
	const char *curr_chaine = NULL;
	unsigned int curr_length = 0;
	std::vector<std::string> ret;

	for (unsigned int i = 0; i < array.size(); i++) {

		std::string curr_string = array[i];
		std::string final;
		for (unsigned int j = 0; j < curr_string.length(); j++) {
			if ((curr_string[j] != ' ') && (curr_string[j] != '\n') && (curr_string[j] != '\t')) {
				final += curr_string[j];
			}
		}
		if (final.length() > 0) {
			ret.push_back(final);
		}
	}
	return ret;
}


void printArray(std::vector<std::string> &array)
{
	for (unsigned int i=0; i<array.size(); i++) {
		std::cerr  << "|" << array[i];
	}
	//std::cerr << "|";
	std::cerr << std::endl;
}



std::vector<std::string> lst_to_array(std::list<string> &channel_array)
{
	std::vector<std::string> ret;
	std::list<string>::iterator it;
	int p = 0;
	for (it = channel_array.begin(); it != channel_array.end(); ++it) {
		ret.push_back(*it);
		p++;
	}
	return ret;
}

std::vector<std::string> array_from_string(std::string mstring)
{
	std::string tmp(mstring);
	std::string curr_string("");
	int curr_index = 0;
	bool making_word = false;
	unsigned int tmp_length = tmp.length();
	std::vector<string> ret;
	for (unsigned int i = 0; i < tmp_length; i++) {
		if ((tmp[i] != ' ') && making_word) {
			// continue current word
			curr_string = curr_string + tmp[i];
		}
		else if ((tmp[i] == ' ') && making_word) { // end of a word
			making_word = false;
			ret.push_back(curr_string);
			curr_string = "";
			curr_index += 1;
		}
		else if ((tmp[i] != ' ') && (!making_word)) {
			// beginning of a new word
			making_word = true;
			curr_string = curr_string + tmp[i];
		}
	}
	//last word
	ret.push_back(curr_string);
	return ret;
}

bool contains_mstring(std::vector<std::string> &m_array, std::string str) {
	return (std::find(m_array.begin(), m_array.end(), str.c_str()) != m_array.end());
	
	/*for (unsigned int i = 0; i < m_array.size(); i++) {
		//cerr << "======[" << m_array[i] << "]" << " compared to " << "[" << str << "]" << endl;
		if (str.compare("MOTION") == 0) {
			cerr << "======[" << m_array[i] << "]" << " compared to " << "[" << str << "]" << endl;
		}
		if (str.compare(m_array[i]) == 0) {
			return true; // return true at first occurence
		}
	}
	return false;*/
}





std::string replace_tab_by_spaces(std::string &str_wtab)
{
	std::string tmp_string = str_wtab;
	std::string ret("");
	for (unsigned int i = 0; i < tmp_string.size(); i++) {
		if ((tmp_string[i] != '\t') &&
		       ((tmp_string[i] == '0') ||
			(tmp_string[i] == '1') ||
			(tmp_string[i] == '2') ||
			(tmp_string[i] == '3') ||
			(tmp_string[i] == '4') ||
			(tmp_string[i] == '5') ||
			(tmp_string[i] == '6') ||
			(tmp_string[i] == '7') ||
			(tmp_string[i] == '8') ||
			(tmp_string[i] == '9') ||
			(tmp_string[i] == '-') ||
			(tmp_string[i] == '.') ||
			(tmp_string[i] == '}') ||
			(tmp_string[i] == '{') ||
			(tmp_string[i] == '_') ||
			(tmp_string[i] == ':') ||	
((tmp_string[i] == 'A') || (tmp_string[i] == 'B') || (tmp_string[i] == 'C') || (tmp_string[i] == 'D') ||
 (tmp_string[i] == 'E') || (tmp_string[i] == 'F') ||
 (tmp_string[i] == 'G') || (tmp_string[i] == 'H') || (tmp_string[i] == 'I') || (tmp_string[i] == 'J') ||
 (tmp_string[i] == 'K') || (tmp_string[i] == 'L') ||
 (tmp_string[i] == 'M') || (tmp_string[i] == 'N') || (tmp_string[i] == 'O') || (tmp_string[i] == 'P') ||
 (tmp_string[i] == 'Q') || (tmp_string[i] == 'R') ||
 (tmp_string[i] == 'S') || (tmp_string[i] == 'T') || (tmp_string[i] == 'U') || (tmp_string[i] == 'V') ||
 (tmp_string[i] == 'W') || (tmp_string[i] == 'X') ||
 (tmp_string[i] == 'Y') || (tmp_string[i] == 'Z') || (tmp_string[i] == 'a') || (tmp_string[i] == 'b') ||
 (tmp_string[i] == 'c') || (tmp_string[i] == 'd') ||
 (tmp_string[i] == 'e') || (tmp_string[i] == 'f') || (tmp_string[i] == 'g') || (tmp_string[i] == 'h') ||
 (tmp_string[i] == 'i') || (tmp_string[i] == 'j') ||
 (tmp_string[i] == 'k') || (tmp_string[i] == 'l') || (tmp_string[i] == 'm') || (tmp_string[i] == 'n') ||
 (tmp_string[i] == 'o') || (tmp_string[i] == 'p') ||
 (tmp_string[i] == 'q') || (tmp_string[i] == 'r') || (tmp_string[i] == 's') || (tmp_string[i] == 't') ||
 (tmp_string[i] == 'u') || (tmp_string[i] == 'v') ||
 (tmp_string[i] == 'w') || (tmp_string[i] == 'x') || (tmp_string[i] == 'y') || (tmp_string[i] == 'z') ) ||
			(tmp_string[i] == '.'))
			) {
			ret = ret + tmp_string[i];
		}
		else {
			ret = ret + ' ';
		}
	}
	return ret;
}


AnimCurve createAnimCurve() {
	AnimCurve retcurve;
	retcurve.name = "uninit";
	retcurve._values.clear();
	return retcurve;
}


Joint* Joint::createFromFile(std::string fileName) {
	Joint* root = NULL;
	cout << "Loading from " << fileName << endl;
	const std::string fname = "";

	const int maxLineSize = 1024;
	char buf[maxLineSize];

	ifstream inputfile(fileName, ios::in);
	if (!inputfile) {
		// open failed
		cerr << fname << ": could not be opened for reading\n";
		exit(1);
	}

	if (!inputfile.getline (buf, maxLineSize)) {
		cout << "file " << fname << " contained no lines ... aborting\n";
		exit(1);
	}
	cerr << "before loosp " << endl;
	bool close_flag = false;
	bool motion_frame = false;

	std::vector<string> channel_array; // list of channels in use

	int time_frame = 0;
	bool change_to_array = true;
	
	Joint *rootNode = NULL;
	Joint *curr_parent = NULL;

	std::vector<AnimCurve> animcurve_tab;
	AnimCurve curr_animcurve;
	Joint *new_joint = NULL;

	// will be used in similar manner to the old channel_array variable
	// but this time it is by pointer, not by name
	std::vector<Joint *> channel_pointer_vector;

	/*************** MAIN LOOP ********************/
	while (inputfile.getline (buf, maxLineSize)) {
		//cerr << "in loop" <<  motion_frame << endl;
		std::string cmdString(buf);
		//cerr << "cmdstring" << cmdString <<  endl;
		cmdString = replace_tab_by_spaces(cmdString);
		//cerr << "cmdstring replaced tab by spaces" << cmdString << endl;
		std::vector<std::string> curr_line_array;
		//cmdString.split(' ', curr_line_array); // TODO replace this for std string
		boost::split(curr_line_array, cmdString, [](char c){return c == ' ';});

		curr_line_array = rstripArray(curr_line_array); // remove whitespaces
		
		std::cerr << endl;
		if (!motion_frame) {
			// if startswith root
			if (curr_line_array[0] == "ROOT") {
				// supports only one root
				rootNode = Joint::create(curr_line_array[1], 0.0, 0.0, 0.0, NULL);
				//name_to_objPt[rootNode->_name] = rootNode;
				curr_parent = rootNode;
			}
			else if (contains_mstring(curr_line_array, "MOTION")) {
				motion_frame = true; // starts reading the keyframes
			}
			else if (contains_mstring(curr_line_array, "JOINT")) {
				new_joint = Joint::create(curr_line_array[curr_line_array.size() - 1],
						0.0, 0.0, 0.0, curr_parent);
				curr_parent = new_joint;
				//name_to_objPt[curr_parent->_name] = curr_parent;
			} else if (contains_mstring(curr_line_array, "End")) { // if contains "End"
				close_flag = true;
			}
			else if (contains_mstring(curr_line_array, "}")) {
				if (close_flag) {
					close_flag = false;
						continue;
				}
				if (curr_parent != NULL) { // if not none
					// we go one level above
					curr_parent = curr_parent->parent; // this assume only one parent
				}

			} else if (contains_mstring(curr_line_array, "CHANNELS")) {
				// curr_line_array should be curr_line.strip().split(" ")
				int chan_nb_param = atoi(curr_line_array[1].c_str());//.asInt(); // rstrip() needed
				printArray(curr_line_array);
				printArray(channel_array);
				cerr << atoi(curr_line_array[1].c_str()) << endl;
				for (int i = 0; i<chan_nb_param; i++) {
					std::string tmp = curr_parent->_name;
					channel_array.push_back(tmp);
					AnimCurve curr_anim;
					animcurve_tab.push_back(createAnimCurve());
					// pointer to object will be repeated in same manner as the name in channel_array
					channel_pointer_vector.push_back(curr_parent);
				}

			} else if (contains_mstring(curr_line_array, "OFFSET")) {
				// curr_offset should be curr_line.strip().split(" ")
				std::string joint_name = curr_parent->_name;
				if (close_flag) {
					joint_name = joint_name + std::string("_tip");
				}
				if ( !(curr_parent == NULL)) {
					//mfn_joint.setRotationOrder(MTransformationMatrix::RotationOrder::kXYZ, false); // reorder = false?
					// update offset
					curr_parent->_rorder = RotateOrder::roXYZ;
					curr_parent->_offX = atof(curr_line_array[1].c_str());
					curr_parent->_offY = atof(curr_line_array[2].c_str());
					curr_parent->_offZ = atof(curr_line_array[3].c_str());
				}
			}
		}
		else { 
			cerr << "frames" << time_frame << endl;
			std::vector<std::string> curr_line_array_frame;
			if (change_to_array) {
				change_to_array = false;
			}

			// skip first two lines of motion section
			if (contains_mstring(curr_line_array, "Frames:")) {
				continue;
			}
			if (contains_mstring(curr_line_array, "Frame")) {
				continue;
			}
	

			std::string cmd_string_wo_tab = replace_tab_by_spaces(cmdString);


			curr_line_array_frame = array_from_string(cmd_string_wo_tab);
			curr_line_array_frame = rstripArray(curr_line_array_frame); // remove whitespaces

			
			for (unsigned int i = 0; i < curr_line_array_frame.size(); i++) { // for each frame at time_frame $time_frame
				Joint * curr_obj = channel_pointer_vector[i];//list_to_objPt[channel_array[i]];
				if (i < 3) { // because root node has 6 channels, the 3 first being translations, the first 3 are translations
					if ((i % 3) == 0) {
						if (time_frame == 0) {
							animcurve_tab[i].name = "translateX";
							animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str())); // add keyframe
							channel_pointer_vector[i]->_dofs.push_back(animcurve_tab[i]); // channel_pointer_vector[i] is the corresponding object
						} else {
							channel_pointer_vector[i]->_dofs[0]._values.push_back(atof(curr_line_array_frame[i].c_str()));
						}
						//animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str())); // add keyframe
					}
					else if ((i % 3) == 1) {

						if (time_frame == 0) {
							//cerr << "###################### create translateY" << endl;
							animcurve_tab[i].name = "translateY";
							animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							channel_pointer_vector[i]->_dofs.push_back(animcurve_tab[i]);
						} else {
							channel_pointer_vector[i]->_dofs[1]._values.push_back(atof(curr_line_array_frame[i].c_str()));
						}
						//animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
					}
					else if ((i % 3) == 2) {
			
						if (time_frame == 0) {
							//cerr << "###################### create translateZ" << endl;
							animcurve_tab[i].name = "translateZ";
							animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							channel_pointer_vector[i]->_dofs.push_back(animcurve_tab[i]);
						} else {
							channel_pointer_vector[i]->_dofs[2]._values.push_back(atof(curr_line_array_frame[i].c_str()));
						}
						//animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
					}
				}
				else { // i>= 3
					if ((i % 3) == 0) {

						if (time_frame == 0) {
							animcurve_tab[i].name = "rotateZ";
							animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							channel_pointer_vector[i]->_dofs.push_back(animcurve_tab[i]);
							//channel_pointer_vector[i]->_dofs[3]._values.push_back(atof(curr_line_array_frame[i].c_str()))
						} else {
							if (i < 6) {							
								channel_pointer_vector[i]->_dofs[3]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							} else {
								channel_pointer_vector[i]->_dofs[0]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							}
						}
						//animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
					}
					else if ((i % 3) == 1) {
						if (time_frame == 0) {
							animcurve_tab[i].name = "rotateY";
							animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							channel_pointer_vector[i]->_dofs.push_back(animcurve_tab[i]);
						} else {
							if (i < 6) {							
								channel_pointer_vector[i]->_dofs[4]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							} else {
								channel_pointer_vector[i]->_dofs[1]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							}
						}
					}
					else if ((i % 3) == 2) {
						if (time_frame == 0) {
							animcurve_tab[i].name = "rotateX";
							animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							channel_pointer_vector[i]->_dofs.push_back(animcurve_tab[i]);
						} else {
							if (i < 6) {							
								channel_pointer_vector[i]->_dofs[5]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							} else {
								channel_pointer_vector[i]->_dofs[2]._values.push_back(atof(curr_line_array_frame[i].c_str()));
							}
						}
						// animcurve_tab[i]._values.push_back(atof(curr_line_array_frame[i].c_str()));
					}
				}
			}
			time_frame += 1; // increase time
		}
	}

	inputfile.close();
	cout << "file closed" << endl;
	/******************* END MAIN LOOP *********************/
	return rootNode;
}



QQuaternion get_rotation_from_angle(float pitch, float yaw, float roll) 
{
	/* return the rotation matrix */
	return (QQuaternion::fromEulerAngles(pitch, yaw, roll).normalized()); // TODO warning rotate order
}

// x y z
void Joint::set_rotate_transform(double pitch, double yaw, double roll)
{

	QMatrix4x4 m_rotateX;
	m_rotateX.rotate(pitch, QVector3D(1.0, 0.0, 0.0));
	QMatrix4x4 m_rotateY;
	m_rotateY.rotate(yaw,   QVector3D(0.0, 1.0, 0.0));
	QMatrix4x4 m_rotateZ;
	m_rotateZ.rotate(roll,  QVector3D(0.0, 0.0, 1.0));
	this->rotate_transform = m_rotateZ * m_rotateY * m_rotateX;	

	//QQuaternion rotate_matrix_quaternion = get_rotation_from_angle(pitch, yaw, roll); // TODO rotation order
	//this->rotate_transform.rotate(rotate_matrix_quaternion);
}

void Joint::set_translate_transform(double tx, double ty, double tz)
{
	QMatrix4x4 m_translate;
	m_translate.translate(tx, ty, tz);
	this->translate_transform = m_translate; //.translate(tx, ty, tz);
}


// with the offsets only
void Joint::initPosition()
{

	// initPosition only deal with the offsets
	QVector4D beginning(0.0, 0.0, 0.0, 1.0);
	QMatrix4x4 parent_translate_transform; // identitys
	if (this->parent == NULL) {
		//cerr << "root has this nb of child  " << this->_children.size() << endl;
		// start from no translation at all for the root
		this->set_translate_transform(_offX, _offY, _offZ);
		this->current_transform = this->translate_transform;
		//this->translate(_offX, _offY, _offZ);
		this->current_position =  (this->current_transform) * beginning;
	} else {
		//cerr << "joint " << this->_name << " has parent " << this->parent->_name << " and nb of children : " <<  this->_children.size() << endl;
		// start where the parent left
		this->current_transform = this->parent->current_transform;
		this->set_translate_transform(_offX, _offY, _offZ);
		this->current_position = this->current_transform * this->translate_transform * beginning;
		this->current_transform = this->current_transform * this->translate_transform; // update current_transform for the children

		// update joint_vertices array for later drawings
		Joint::joint_vertices.push_back(trimesh::point(this->parent->current_position.x(), this->parent->current_position.y(), this->parent->current_position.z(), this->parent->current_position.w()));
		Joint::joint_vertices.push_back(trimesh::point(this->current_position.x(), this->current_position.y(), this->current_position.z(), this->parent->current_position.w()));
	}

	// propagate to children
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
			_children[ichild]->initPosition();
	}
}

// to obtain position do [x0, y0, z0, 1.0] * R * T * S where scale is identity
// for now we will entirely avoid keyframe and only deal with the offset to get an initial representation
void Joint::animate(int iframe)
{
	//cerr << "iframe is : " <<  iframe << endl;
	//cerr << "dof_size" << _dofs.size() << endl;
	if (this->parent == NULL) {
		Joint::joint_vertices.clear();
	}
	
	// Update dofs :
	_curTx = 0; _curTy = 0; _curTz = 0;
	_curRx = 0; _curRy = 0; _curRz = 0;

	for (unsigned int idof = 0 ; idof < _dofs.size() ; idof++) {
		// _dofs[idof] corresponds to idof-th degree of freedom
		// it is an AnimCurve
		// each AnimCurve contains member _values which is the std::vector<float>
		// of the keyframes associated with this dof for this joint
		// cerr << _dofs[idof]._values.size() << endl;
		if(!_dofs[idof].name.compare("translateX")) _curTx = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("translateY")) _curTy = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("translateZ")) _curTz = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("rotateZ")) _curRz = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("rotateY")) _curRy = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("rotateX")) _curRx = _dofs[idof]._values[iframe];
		/*if(!_dofs[idof].name.compare("rotateY")) {
			cerr << this->_name << " rotateY channel contains : " << _dofs[idof]._values.size() << " frames and for frame_id " << iframe << " value is "  << _dofs[idof]._values[iframe] << endl;
		}*/
	}

	QMatrix4x4 init_rotate_transform_matrix; // initialize to identity
	QMatrix4x4 init_translate_transform_matrix; // initialize to identity

	/*
	if (this->parent != NULL) { // if parent is not NULL then compose current created from keyframe with transform from parent
		init_rotate_transform_matrix = this->parent->rotate_transform; // call copy constructor
		this->rotate_transform = init_rotate_transform_matrix; // begin where the parent left
		init_translate_transform_matrix = this->parent->translate_transform; // call copy constructor
		this->translate_transform = init_translate_transform_matrix; // begin where the parent left
	} else {
		// it means we're at the root node then
		// keep the identity matrix
		this->translate_transform = init_translate_transform_matrix;
		this->rotate_transform = init_rotate_transform_matrix; // initialize to identity if parent == NULL
	}*/



	if (this->parent == NULL) {
		this->set_translate_transform(_offX, _offY, _offZ);
		this->translate_transform.translate(_curTx, _curTy, _curTz);
		this->set_rotate_transform(_curRx, _curRy, _curRz);
		this->current_transform = this->translate_transform * this->rotate_transform;
	} else {
		this->set_translate_transform(_offX, _offY, _offZ);
		this->set_rotate_transform(_curRx, _curRy, _curRz);
		this->current_transform = this->parent->current_transform * this->translate_transform * this->rotate_transform;
	}
	
	QVector4D beginning(0.0, 0.0, 0.0, 1.0);
	this->current_position = this->current_transform * beginning;


	/*
	// apply transform to account for the current keyframe values in each channel
	// (ie for each degree of freedom)
	// TODO which order, rotate before translate? keep separate?
	if (this->parent == NULL) { // only the root has translation channels with keyframes
		//this->translate_transform = parent_translate_transform_matrix; // this is identity !
		this->translate(_curTx, _curTy, _curTz); // add the keyframes of translation of the root joint
	}

	// also add the offsets to translate_transform always do it, for root or other alike
	// every frame it is properly reset since from the root the transforms are reinitialised to identity
	// then update with the frame values for translate and rotate + the offset
	// and then for every child the transform will be initialised to the parent (the one we just redefine)
	// so we end up properly redefining the transforms in the entire hierarchy
	this->translate(_offX, _offY, _offZ);

	// always do this since all joints have 3 rotate channels
	this->rotate(_curRy, _curRz, _curRx); // TODO order of rotation
	
	// update world position
	// cerr << rotate_transform << endl;
	QVector4D curr_position(0.0, 0.0, 0.0, 1.0);
	this->current_position =  (this->translate_transform) * (this->rotate_transform) * curr_position;
	*/
	
	// update vertices array used to draw the frame
	if (this->parent != NULL) {
		Joint::joint_vertices.push_back(trimesh::point(this->parent->current_position.x(),
						this->parent->current_position.y(),
						this->parent->current_position.z(), this->current_position.w()));
		Joint::joint_vertices.push_back(trimesh::point(this->current_position.x(),
						this->current_position.y(),
						this->current_position.z(), this->current_position.w()));
	}
	
	// Animate children :
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->animate(iframe);
	}
} 


void Joint::nbDofs() {
	if (_dofs.empty()) return;

	double tol = 1e-4;

	int nbDofsR = -1;

	// TODO :
	cout << _name << " : " << nbDofsR << " degree(s) of freedom in rotation\n";

	// Propagate to children :
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->nbDofs();
	}

}
