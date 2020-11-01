#include "shapes.h"
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

Shapes::Shapes() {

};

Shapes::~Shapes() {

}

void Shapes::LoadObj() {

	std::vector< glm::vec3 > obj_vertices;
	std::vector< unsigned int > vertexIndices;
	istringstream rawDataStream(rawData);
	string dataLine;  int linesDone = 0;

	while (std::getline(rawDataStream, dataLine)) {
		if (dataLine.find("v ") != string::npos) {	// does this line have a vector?
			glm::vec3 vertex;

			int foundStart = dataLine.find(" ");  int foundEnd = dataLine.find(" ", foundStart + 1);
			vertex.x = stof(dataLine.substr(foundStart, foundEnd - foundStart));

			foundStart = foundEnd; foundEnd = dataLine.find(" ", foundStart + 1);
			vertex.y = stof(dataLine.substr(foundStart, foundEnd - foundStart));

			foundStart = foundEnd; foundEnd = dataLine.find(" ", foundStart + 1);
			vertex.z = stof(dataLine.substr(foundStart, foundEnd - foundStart));

			obj_vertices.push_back(vertex);
		}
		else if (dataLine.find("f ") != string::npos) { // does this line defines a triangle face?
			string parts[3];

			int foundStart = dataLine.find(" ");  int foundEnd = dataLine.find(" ", foundStart + 1);
			parts[0] = dataLine.substr(foundStart + 1, foundEnd - foundStart - 1);

			foundStart = foundEnd; foundEnd = dataLine.find(" ", foundStart + 1);
			parts[1] = dataLine.substr(foundStart + 1, foundEnd - foundStart - 1);

			foundStart = foundEnd; foundEnd = dataLine.find(" ", foundStart + 1);
			parts[2] = dataLine.substr(foundStart + 1, foundEnd - foundStart - 1);

			for (int i = 0; i < 3; i++) {		// for each part

				vertexIndices.push_back(stoul(parts[i].substr(0, parts[i].find("/"))));

				int firstSlash = parts[i].find("/"); int secondSlash = parts[i].find("/", firstSlash + 1);

				if (firstSlash != (secondSlash + 1)) {	// there is texture coordinates.
														// add code for my texture coordintes here.
				}
			}
		}

		linesDone++;
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i += 3) {
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 0] - 1].x);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 0] - 1].y);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 0] - 1].z);

		vertexPositions.push_back(obj_vertices[vertexIndices[i + 1] - 1].x);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 1] - 1].y);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 1] - 1].z);

		vertexPositions.push_back(obj_vertices[vertexIndices[i + 2] - 1].x);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 2] - 1].y);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 2] - 1].z);
	}
}


void Shapes::Load() {
	const char* vs_source[] = { R"(
#version 330 core

in vec4 position;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void){
	gl_Position = proj_matrix * mv_matrix * position;
}
)" };


	const char* fs_source[] = { R"(
#version 330 core

uniform vec4 inColor;
out vec4 color;

void main(void){
	color = inColor;
}
)" };

	program = glCreateProgram();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);
	checkErrorShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);
	checkErrorShader(vs);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	mv_location = glGetUniformLocation(program, "mv_matrix");
	proj_location = glGetUniformLocation(program, "proj_matrix");
	color_location = glGetUniformLocation(program, "inColor");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
		vertexPositions.size() * sizeof(GLfloat),
		&vertexPositions[0],
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glLinkProgram(0);	// unlink
	glDisableVertexAttribArray(0); // Disable
	glBindVertexArray(0);	// Unbind
}

void Shapes::Draw() {
	glUseProgram(program);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	glUniformMatrix4fv(proj_location, 1, GL_FALSE, &proj_matrix[0][0]);
	glUniformMatrix4fv(mv_location, 1, GL_FALSE, &mv_matrix[0][0]);

	glUniform4f(color_location, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertexPositions.size() / 3);

	glUniform4f(color_location, lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  glLineWidth(lineWidth);
	glDrawArrays(GL_TRIANGLES, 0, vertexPositions.size() / 3);
}


void Shapes::checkErrorShader(GLuint shader) {
	// Get log length
	GLint maxLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// Init a string for it
	std::vector<GLchar> errorLog(maxLength);

	if (maxLength > 1) {
		// Get the log file
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		cout << "--------------Shader compilation error-------------\n";
		cout << errorLog.data();
	}
}

Specific::Specific()
{
	rawData = R"(
o Spider
v 5.275970 48.778538 13.657825
v 6.511691 60.279461 42.827667
v 4.974944 49.771633 80.855789
v 12.603921 43.686905 69.494820
v 4.550550 44.040253 80.092903
v 16.122772 43.306496 47.762672
v 12.487747 43.538456 26.115288
v 4.418329 43.805847 16.271925
v 5.137498 38.839512 67.976524
v 6.578961 36.972897 46.350449
v 5.036498 38.587662 26.144516
v 14.640574 55.280708 26.062668
v 15.873369 50.368629 26.267851
v 11.994610 51.411644 17.354383
v 5.534987 53.212318 14.671083
v 6.218451 58.241783 23.330044
v 15.771140 56.604404 45.348408
v 18.803246 50.820011 46.771652
v 14.317750 55.868416 64.542351
v 11.443506 52.442341 74.730530
v 15.796528 50.546078 67.711494
v 5.987620 58.964428 63.210625
v 5.036464 54.872784 74.961281
v -5.275974 48.778538 13.657820
v -6.511717 60.279461 42.827667
v -4.975002 49.771633 80.855789
v -12.603966 43.686901 69.494797
v -4.550608 44.040253 80.092918
v -16.122803 43.306488 47.762653
v -12.487761 43.538452 26.115276
v -4.418333 43.805847 16.271919
v -5.137545 38.839512 67.976524
v -6.578990 36.972897 46.350449
v -5.036511 38.587662 26.144516
v -14.640588 55.280704 26.062658
v -15.873380 50.368622 26.267841
v -11.994615 51.411636 17.354376
v -5.534991 53.212315 14.671078
v -6.218462 58.241783 23.330038
v -15.771166 56.604401 45.348389
v -18.803276 50.820007 46.771633
v -14.317795 55.868412 64.542336
v -11.443559 52.442337 74.730530
v -15.796574 50.546074 67.711479
v -5.987664 58.964428 63.210625
v -5.036518 54.872784 74.961281
v -0.036661 43.662258 -23.736126
v -0.024433 48.298454 -29.197580
v -0.024439 39.723209 -16.297764
v -0.089807 43.792221 -26.281498
v -7.192308 46.414421 -29.314434
v -1.516717 46.568947 -30.485451
v -4.590011 47.203346 -29.305676
v -5.919268 45.760361 -30.231535
v -2.493414 45.812382 -30.751059
v -7.265184 23.142334 -46.148518
v -11.680996 26.930735 -40.866524
v -8.718949 25.021046 -43.135666
v -8.319749 23.878286 -45.882797
v -12.331538 27.076340 -42.494884
v -11.914931 29.152504 -42.993534
v -7.432902 23.888947 -46.692802
v -9.081877 26.927912 -45.254799
v -16.295736 38.555927 -33.404686
v -13.966550 32.178551 -40.014984
v -14.053575 35.769306 -37.814331
v -15.489339 31.402929 -37.872021
v -17.475002 36.518291 -32.178406
v -13.839783 29.673471 -37.681259
v -16.203087 35.436054 -30.370632
v -13.551384 32.055790 -33.618847
v -5.742169 22.693014 -47.486488
v -8.069224 26.381268 -44.148346
v -12.264524 33.961784 -36.053249
v -13.284377 39.686699 -23.761980
v -15.674126 42.323738 -24.615643
v -11.400743 41.042786 -26.632153
v -13.790493 43.660946 -27.498711
v -11.059885 48.504520 -22.081524
v -9.565841 48.399086 -24.905029
v -4.791413 50.130062 -25.268728
v -4.749697 48.593864 -27.336733
v -0.720435 40.095943 -25.178753
v -4.051255 39.004063 -20.412451
v -3.940109 38.404766 -25.956623
v -3.885066 39.739666 -29.389593
v -6.730361 42.302246 -29.475819
v -1.169410 42.452232 -29.955675
v -10.135583 50.963329 -20.835030
v -4.594720 51.224678 -23.347736
v -7.574410 42.295410 -27.255943
v -8.471634 43.219910 -23.581211
v -3.362508 39.493439 -16.453182
v -13.606138 48.626923 -17.292744
v -14.041161 45.883492 -16.403347
v -11.117914 42.715904 -15.036341
v -3.235129 38.292816 -9.513474
v -10.229403 51.770893 -17.545851
v -12.351698 50.741013 -17.322191
v -4.398027 52.284111 -19.524670
v -7.405147 39.200058 -11.761824
v -2.458975 40.663647 27.656878
v -4.917941 40.663647 22.065262
v -1.997393 38.914833 22.135952
v -2.920552 42.412460 22.481277
v -3.891896 44.902145 15.890909
v -3.176469 38.678421 15.199022
v -7.084183 41.864815 16.069870
v -4.668552 50.638348 10.823025
v -4.680235 51.589237 8.365417
v -11.101324 50.497276 10.959736
v -13.624247 50.544514 8.778965
v -11.291889 51.352428 8.495836
v -14.805860 50.788551 3.834735
v -11.945007 51.987381 3.815194
v -4.478573 52.828938 -13.735023
v -14.774179 50.796021 -3.891580
v -11.764219 52.335594 -4.333837
v -13.970637 50.773014 -11.587320
v -11.111662 52.060314 -12.161200
v -3.957014 38.239170 -1.380403
v -4.154657 38.317337 10.931805
v -3.965472 38.285103 4.636616
v -11.769209 44.344219 14.511522
v -15.586515 45.761101 -9.370100
v -16.541235 45.235077 -2.241833
v -14.962426 45.403782 11.178345
v -16.207603 45.227730 4.601555
v -4.826584 52.591042 3.648117
v -16.146246 48.803707 3.985969
v -14.713674 48.692581 9.851974
v -4.698872 53.244404 -5.179348
v -15.391230 48.807655 -10.682251
v -16.358480 48.778538 -3.243425
v -11.742611 48.667408 13.004166
v -4.827210 48.778538 12.807108
v 0.036689 43.662262 -23.736126
v 0.024467 48.298454 -29.197580
v 0.024461 39.723209 -16.297764
v 0.089838 43.792221 -26.281498
v 7.192344 46.414417 -29.314425
v 1.516753 46.568951 -30.485451
v 4.590045 47.203346 -29.305679
v 5.919304 45.760361 -30.231541
v 2.493450 45.812386 -30.751053
v 7.265233 23.142340 -46.148521
v 11.681043 26.930735 -40.866524
v 8.718995 25.021046 -43.135666
v 8.319798 23.878288 -45.882797
v 12.331583 27.076342 -42.494865
v 11.914978 29.152512 -42.993534
v 7.432952 23.888948 -46.692802
v 9.081926 26.927912 -45.254807
v 16.295771 38.555931 -33.404678
v 13.966591 32.178558 -40.014980
v 14.053617 35.769310 -37.814327
v 15.489379 31.402935 -37.872009
v 17.475039 36.518295 -32.178398
v 13.839825 29.673477 -37.681252
v 16.203125 35.436062 -30.370619
v 13.551424 32.055798 -33.618847
v 5.742220 22.693020 -47.486488
v 8.069271 26.381271 -44.148354
v 12.264565 33.961792 -36.053238
v 13.284410 39.686703 -23.761971
v 15.674158 42.323742 -24.615633
v 11.400775 41.042789 -26.632145
v 13.790525 43.660950 -27.498699
v 11.059915 48.504524 -22.081516
v 9.565871 48.399090 -24.905022
v 4.791443 50.130062 -25.268728
v 4.749730 48.593864 -27.336727
v 0.720463 40.095943 -25.178753
v 4.051281 39.004063 -20.412445
v 3.940136 38.404766 -25.956623
v 3.885096 39.739666 -29.389593
v 6.730392 42.302246 -29.475815
v 1.169443 42.452232 -29.955675
v 10.135607 50.963333 -20.835020
v 4.594748 51.224686 -23.347736
v 7.574441 42.295403 -27.255938
v 8.471663 43.219910 -23.581207
v 3.362531 39.493443 -16.453178
v 13.606162 48.626923 -17.292736
v 14.041185 45.883492 -16.403339
v 11.117933 42.715897 -15.036333
v 3.235145 38.292820 -9.513470
v 10.229426 51.770893 -17.545841
v 12.351722 50.741020 -17.322184
v 4.398053 52.284111 -19.524666
v 7.405165 39.200058 -11.761816
v 2.458961 40.663647 27.656878
v 4.917931 40.663647 22.065262
v 1.997383 38.914833 22.135952
v 2.920543 42.412460 22.481277
v 3.891891 44.902145 15.890915
v 3.176465 38.678421 15.199024
v 7.084179 41.864815 16.069874
v 4.668553 50.638348 10.823029
v 4.680237 51.589237 8.365421
v 11.101324 50.497276 10.959745
v 13.624247 50.544518 8.778975
v 11.291894 51.352432 8.495846
v 14.805864 50.788555 3.834748
v 11.945013 51.987385 3.815204
v 4.478593 52.828938 -13.735020
v 14.774194 50.796021 -3.891567
v 11.764233 52.335602 -4.333827
v 13.970656 50.773014 -11.587313
v 11.111681 52.060318 -12.161190
v 3.968598 38.188904 -1.397199
v 4.154655 38.317341 10.931808
v 3.967616 38.216839 4.620357
v 11.769209 44.344223 14.511530
v 15.564681 45.809170 -9.348820
v 16.512278 45.267532 -2.220593
v 14.936973 45.441307 11.162329
v 16.172655 45.260307 4.583845
v 4.826591 52.591042 3.648121
v 16.146252 48.803711 3.985983
v 14.713673 48.692581 9.851984
v 4.698884 53.244404 -5.179345
v 15.391247 48.807655 -10.682238
v 16.358490 48.778538 -3.243412
v 11.742610 48.667412 13.004177
v 4.827208 48.778538 12.807114
v 15.583342 45.293633 7.870243
v 15.325468 43.151684 4.994614
v 16.531921 45.228844 1.245104
v 13.801678 45.262939 13.662001
v 16.161251 45.398693 -5.779400
v 14.995485 44.312378 -8.932892
v 14.805576 46.072845 -12.929241
v 9.350851 43.080399 14.916016
v 8.298230 38.166298 10.832731
v 7.899535 37.555492 -1.172605
v 5.727510 39.560997 15.527834
v 8.798306 40.736710 -13.756128
v 5.878360 38.371120 -9.968559
v 13.239957 44.560005 -15.887131
v -15.364483 43.099785 5.017898
v -16.531904 45.228844 1.245091
v -13.801678 45.262939 13.661989
v -14.417730 43.406162 11.762077
v -15.677448 43.151024 -1.684216
v -11.370721 41.305370 14.662331
v -11.508728 43.061783 -10.905884
v -7.876068 37.676121 4.603116
v -8.298233 38.166290 10.832731
v -3.998580 38.219582 7.736128
v -7.865844 37.607689 -1.144707
v -3.767617 38.187939 -4.862284
v -8.176347 39.099270 -6.436369
v -8.798285 40.736713 -13.756134
v -5.878347 38.371120 -9.968569
v 3.767629 38.187946 -4.862277
v 3.998582 38.219589 7.736128
v 3.962461 38.213783 1.569023
v 4.009966 38.472988 13.587429
v 9.593616 49.436813 14.933201
v 14.541526 47.163067 25.567753
v 6.462627 59.742397 31.344921
v 12.082404 58.691654 44.362404
v 18.275307 47.217922 47.375221
v 5.075874 47.148888 81.320122
v 14.566522 47.254158 69.700699
v 9.079572 50.473484 78.544746
v 15.235027 43.415127 59.512074
v 9.572864 40.622597 68.463531
v 8.839182 43.951912 76.694038
v 15.222048 43.350948 36.034458
v 12.254400 39.690041 47.549316
v 8.644849 43.734818 19.147121
v 9.417562 40.383686 26.708267
v 6.210565 37.448277 57.548306
v 18.082899 50.686134 34.903473
v 15.524863 56.229614 33.741199
v 9.917388 52.587975 15.736496
v 11.433209 56.979004 24.61496

)";

LoadObj();
}

Specific::~Specific()
{

}

Cube::Cube() {
	// Exported from Blender a cube by default (OBJ File)
	rawData = R"(
v 0.500000 -0.500000 -0.500000
v 0.500000 -0.500000 0.500000
v -0.500000 -0.500000 0.500000
v -0.500000 -0.500000 -0.500000
v 0.500000 0.500000 -0.499999
v 0.499999 0.500000 0.500000
v -0.500000 0.500000 0.500000
v -0.500000 0.500000 -0.500000
f 1 3 4
f 8 6 5
f 5 2 1
f 6 3 2
f 7 4 3
f 1 8 5
f 1 2 3
f 8 7 6
f 5 6 2
f 6 7 3
f 7 8 4
f 1 4 8)";

	LoadObj();
}


Cube::~Cube() {

}

Sphere::Sphere() {

	rawData = R"(
o Sphere
v 0.000000 -0.154509 0.475528
v 0.000000 -0.404509 0.293893
v -0.090818 0.475528 0.125000
v -0.172746 0.404509 0.237764
v -0.237764 0.293893 0.327254
v -0.279509 0.154508 0.384710
v -0.293893 -0.000000 0.404508
v -0.279509 -0.154509 0.384710
v -0.237764 -0.293893 0.327254
v -0.172746 -0.404509 0.237764
v -0.090818 -0.475528 0.125000
v -0.146946 0.475528 0.047746
v -0.279509 0.404509 0.090818
v -0.384711 0.293893 0.125000
v -0.452254 0.154508 0.146946
v -0.475528 -0.000000 0.154508
v -0.452254 -0.154509 0.146946
v -0.384711 -0.293893 0.125000
v -0.279509 -0.404509 0.090818
v -0.146946 -0.475528 0.047746
v -0.146946 0.475528 -0.047746
v -0.279509 0.404509 -0.090818
v -0.384711 0.293893 -0.125000
v -0.452254 0.154508 -0.146946
v -0.475528 -0.000000 -0.154509
v -0.452254 -0.154509 -0.146947
v -0.384711 -0.293893 -0.125000
v -0.279509 -0.404509 -0.090818
v -0.146946 -0.475528 -0.047746
v -0.090818 0.475528 -0.125000
v -0.172746 0.404509 -0.237764
v -0.237764 0.293893 -0.327255
v -0.279509 0.154508 -0.384711
v -0.293893 -0.000000 -0.404509
v -0.279509 -0.154509 -0.384711
v -0.237764 -0.293893 -0.327255
v -0.172746 -0.404509 -0.237764
v -0.090818 -0.475528 -0.125000
v -0.000000 0.475528 -0.154509
v -0.000000 0.404509 -0.293893
v 0.000000 0.293893 -0.404509
v -0.000000 0.154508 -0.475529
v -0.000000 -0.000000 -0.500000
v 0.000000 -0.154509 -0.475529
v 0.000000 -0.293893 -0.404509
v -0.000000 -0.404509 -0.293893
v -0.000000 -0.475528 -0.154509
v 0.000000 0.500000 -0.000000
v 0.090818 0.475528 -0.125000
v 0.172746 0.404509 -0.237764
v 0.237764 0.293893 -0.327255
v 0.279509 0.154508 -0.384711
v 0.293893 -0.000000 -0.404509
v 0.279509 -0.154509 -0.384711
v 0.237764 -0.293893 -0.327255
v 0.172746 -0.404509 -0.237764
v 0.090818 -0.475528 -0.125000
v 0.146946 0.475528 -0.047746
v 0.279509 0.404509 -0.090818
v 0.384711 0.293893 -0.125000
v 0.452254 0.154508 -0.146947
v 0.475528 -0.000000 -0.154509
v 0.452254 -0.154509 -0.146947
v 0.384711 -0.293893 -0.125000
v 0.279509 -0.404509 -0.090818
v 0.146946 -0.475528 -0.047746
v 0.146946 0.475528 0.047746
v 0.279509 0.404509 0.090818
v 0.384711 0.293893 0.125000
v 0.452254 0.154508 0.146946
v 0.475528 -0.000000 0.154508
v 0.452254 -0.154509 0.146946
v 0.384711 -0.293893 0.125000
v 0.279509 -0.404509 0.090818
v 0.146946 -0.475528 0.047746
v 0.090818 0.475528 0.125000
v 0.172746 0.404509 0.237764
v 0.237764 0.293893 0.327254
v 0.279509 0.154508 0.384710
v 0.293893 -0.000000 0.404508
v 0.279509 -0.154509 0.384710
v 0.237764 -0.293893 0.327254
v 0.172746 -0.404509 0.237764
v 0.090818 -0.475528 0.125000
v 0.000000 0.475528 0.154509
v 0.000000 0.404509 0.293893
v 0.000000 0.293893 0.404509
v 0.000000 0.154508 0.475528
v 0.000000 -0.000000 0.500000
v 0.000000 -0.293893 0.404509
v 0.000000 -0.475528 0.154508
v 0.000000 -0.500000 -0.000000
vt 0.700000 0.400000
vt 0.600000 0.500000
vt 0.600000 0.400000
vt 0.700000 0.800000
vt 0.600000 0.700000
vt 0.700000 0.700000
vt 0.650000 0.000000
vt 0.700000 0.100000
vt 0.600000 0.100000
vt 0.600000 0.300000
vt 0.700000 0.300000
vt 0.600000 0.600000
vt 0.700000 0.600000
vt 0.700000 0.900000
vt 0.650000 1.000000
vt 0.600000 0.900000
vt 0.700000 0.200000
vt 0.600000 0.200000
vt 0.700000 0.500000
vt 0.600000 0.800000
vt 0.500000 0.700000
vt 0.550000 0.000000
vt 0.500000 0.100000
vt 0.500000 0.300000
vt 0.500000 0.600000
vt 0.550000 1.000000
vt 0.500000 0.900000
vt 0.500000 0.200000
vt 0.500000 0.500000
vt 0.500000 0.800000
vt 0.500000 0.400000
vt 0.400000 0.200000
vt 0.400000 0.100000
vt 0.400000 0.500000
vt 0.400000 0.400000
vt 0.400000 0.700000
vt 0.450000 0.000000
vt 0.400000 0.300000
vt 0.400000 0.600000
vt 0.450000 1.000000
vt 0.400000 0.900000
vt 0.400000 0.800000
vt 0.300000 0.600000
vt 0.300000 0.500000
vt 0.300000 0.800000
vt 0.300000 0.100000
vt 0.300000 0.400000
vt 0.300000 0.700000
vt 0.350000 0.000000
vt 0.300000 0.300000
vt 0.350000 1.000000
vt 0.300000 0.900000
vt 0.300000 0.200000
vt 0.250000 1.000000
vt 0.200000 0.900000
vt 0.200000 0.200000
vt 0.200000 0.500000
vt 0.200000 0.800000
vt 0.200000 0.100000
vt 0.200000 0.400000
vt 0.200000 0.700000
vt 0.250000 0.000000
vt 0.200000 0.300000
vt 0.200000 0.600000
vt 0.100000 0.600000
vt 0.150000 1.000000
vt 0.100000 0.900000
vt 0.100000 0.200000
vt 0.100000 0.500000
vt 0.100000 0.800000
vt 0.100000 0.100000
vt 0.100000 0.400000
vt 0.100000 0.700000
vt 0.150000 0.000000
vt 0.100000 0.300000
vt 0.050000 0.000000
vt 0.000000 0.100000
vt 0.000000 0.300000
vt 0.000000 0.600000
vt 0.050000 1.000000
vt 0.000000 0.900000
vt 0.000000 0.200000
vt 0.000000 0.500000
vt 0.000000 0.800000
vt 0.000000 0.400000
vt 0.000000 0.700000
vt 1.000000 0.400000
vt 0.900000 0.500000
vt 0.900000 0.400000
vt 1.000000 0.700000
vt 0.900000 0.800000
vt 0.900000 0.700000
vt 0.950000 0.000000
vt 1.000000 0.100000
vt 0.900000 0.100000
vt 0.900000 0.300000
vt 1.000000 0.300000
vt 0.900000 0.600000
vt 1.000000 0.600000
vt 1.000000 0.900000
vt 0.950000 1.000000
vt 0.900000 0.900000
vt 0.900000 0.200000
vt 1.000000 0.200000
vt 1.000000 0.500000
vt 1.000000 0.800000
vt 0.800000 0.800000
vt 0.800000 0.200000
vt 0.800000 0.100000
vt 0.800000 0.500000
vt 0.800000 0.400000
vt 0.800000 0.700000
vt 0.850000 0.000000
vt 0.800000 0.300000
vt 0.800000 0.600000
vt 0.850000 1.000000
vt 0.800000 0.900000
vt 0.750000 0.000000
vt 0.750000 1.000000
vn -0.3056 -0.1490 0.9404
vn -0.2239 0.6892 0.6892
vn -0.0508 -0.9864 0.1562
vn -0.2781 -0.4361 0.8559
vn -0.2781 0.4361 0.8559
vn -0.0508 0.9864 0.1562
vn -0.2239 -0.6892 0.6892
vn -0.3056 0.1490 0.9404
vn -0.1459 0.8815 0.4491
vn -0.1459 -0.8815 0.4491
vn -0.5862 0.6892 0.4259
vn -0.1329 -0.9864 0.0966
vn -0.7280 -0.4361 0.5290
vn -0.7280 0.4361 0.5290
vn -0.1329 0.9864 0.0966
vn -0.5862 -0.6892 0.4259
vn -0.8000 0.1490 0.5812
vn -0.3821 0.8815 0.2776
vn -0.3821 -0.8815 0.2776
vn -0.8000 -0.1490 0.5812
vn -0.4722 -0.8815 0.0000
vn -0.9888 -0.1490 0.0000
vn -0.7246 0.6892 0.0000
vn -0.1643 -0.9864 0.0000
vn -0.8999 -0.4361 0.0000
vn -0.8999 0.4361 0.0000
vn -0.1643 0.9864 0.0000
vn -0.7246 -0.6892 0.0000
vn -0.9888 0.1490 0.0000
vn -0.4722 0.8815 0.0000
vn -0.8000 0.1490 -0.5812
vn -0.3821 0.8815 -0.2776
vn -0.3821 -0.8815 -0.2776
vn -0.8000 -0.1490 -0.5812
vn -0.5862 0.6892 -0.4259
vn -0.1329 -0.9864 -0.0966
vn -0.7280 -0.4361 -0.5290
vn -0.7280 0.4361 -0.5290
vn -0.1329 0.9864 -0.0966
vn -0.5862 -0.6892 -0.4259
vn -0.0508 0.9864 -0.1562
vn -0.2239 -0.6892 -0.6892
vn -0.3056 0.1490 -0.9404
vn -0.1459 0.8815 -0.4491
vn -0.1459 -0.8815 -0.4491
vn -0.3056 -0.1490 -0.9404
vn -0.2239 0.6892 -0.6892
vn -0.0508 -0.9864 -0.1562
vn -0.2781 -0.4361 -0.8559
vn -0.2781 0.4361 -0.8559
vn 0.2781 0.4361 -0.8559
vn 0.0508 0.9864 -0.1562
vn 0.2239 -0.6892 -0.6892
vn 0.3056 0.1490 -0.9404
vn 0.1459 0.8815 -0.4491
vn 0.1459 -0.8815 -0.4491
vn 0.3056 -0.1490 -0.9404
vn 0.2239 0.6892 -0.6892
vn 0.0508 -0.9864 -0.1562
vn 0.2781 -0.4361 -0.8559
vn 0.1329 -0.9864 -0.0966
vn 0.7280 -0.4361 -0.5290
vn 0.7280 0.4361 -0.5290
vn 0.1329 0.9864 -0.0966
vn 0.5862 -0.6892 -0.4259
vn 0.8000 0.1490 -0.5812
vn 0.3821 0.8815 -0.2776
vn 0.3821 -0.8815 -0.2776
vn 0.8000 -0.1490 -0.5812
vn 0.5862 0.6892 -0.4259
vn 0.9888 -0.1490 0.0000
vn 0.7246 0.6892 0.0000
vn 0.1643 -0.9864 -0.0000
vn 0.8999 -0.4361 0.0000
vn 0.8999 0.4361 -0.0000
vn 0.1643 0.9864 0.0000
vn 0.7246 -0.6892 -0.0000
vn 0.9888 0.1490 -0.0000
vn 0.4722 0.8815 0.0000
vn 0.4722 -0.8815 0.0000
vn 0.3821 0.8815 0.2776
vn 0.3821 -0.8815 0.2776
vn 0.8000 -0.1490 0.5812
vn 0.5862 0.6892 0.4259
vn 0.1329 -0.9864 0.0966
vn 0.7280 -0.4361 0.5290
vn 0.7280 0.4361 0.5290
vn 0.1329 0.9864 0.0966
vn 0.5862 -0.6892 0.4259
vn 0.8000 0.1490 0.5812
vn 0.3056 0.1490 0.9404
vn 0.1459 0.8815 0.4491
vn 0.1459 -0.8815 0.4491
vn 0.3056 -0.1490 0.9404
vn 0.2239 0.6892 0.6892
vn 0.0508 -0.9864 0.1562
vn 0.2781 -0.4361 0.8559
vn 0.2781 0.4361 0.8559
vn 0.0508 0.9864 0.1562
vn 0.2239 -0.6892 0.6892
s off
f 1/1/1 7/2/1 8/3/1
f 86/4/2 5/5/2 87/6/2
f 92/7/3 91/8/3 11/9/3
f 1/1/4 9/10/4 90/11/4
f 87/6/5 6/12/5 88/13/5
f 85/14/6 48/15/6 3/16/6
f 2/17/7 9/10/7 10/18/7
f 88/13/8 7/2/8 89/19/8
f 85/14/9 4/20/9 86/4/9
f 91/8/10 10/18/10 11/9/10
f 4/20/11 14/21/11 5/5/11
f 92/22/12 11/9/12 20/23/12
f 8/3/13 18/24/13 9/10/13
f 6/12/14 14/21/14 15/25/14
f 3/16/15 48/26/15 12/27/15
f 9/10/16 19/28/16 10/18/16
f 6/12/17 16/29/17 7/2/17
f 3/16/18 13/30/18 4/20/18
f 10/18/19 20/23/19 11/9/19
f 8/3/20 16/29/20 17/31/20
f 20/23/21 28/32/21 29/33/21
f 17/31/22 25/34/22 26/35/22
f 13/30/23 23/36/23 14/21/23
f 92/37/24 20/23/24 29/33/24
f 17/31/25 27/38/25 18/24/25
f 14/21/26 24/39/26 15/25/26
f 12/27/27 48/40/27 21/41/27
f 18/24/28 28/32/28 19/28/28
f 15/25/29 25/34/29 16/29/29
f 12/27/30 22/42/30 13/30/30
f 25/34/31 33/43/31 34/44/31
f 21/41/32 31/45/32 22/42/32
f 28/32/33 38/46/33 29/33/33
f 26/35/34 34/44/34 35/47/34
f 23/36/35 31/45/35 32/48/35
f 92/49/36 29/33/36 38/46/36
f 26/35/37 36/50/37 27/38/37
f 23/36/38 33/43/38 24/39/38
f 21/41/39 48/51/39 30/52/39
f 27/38/40 37/53/40 28/32/40
f 30/52/41 48/54/41 39/55/41
f 36/50/42 46/56/42 37/53/42
f 33/43/43 43/57/43 34/44/43
f 30/52/44 40/58/44 31/45/44
f 37/53/45 47/59/45 38/46/45
f 35/47/46 43/57/46 44/60/46
f 32/48/47 40/58/47 41/61/47
f 92/62/48 38/46/48 47/59/48
f 35/47/49 45/63/49 36/50/49
f 32/48/50 42/64/50 33/43/50
f 41/61/51 52/65/51 42/64/51
f 39/55/52 48/66/52 49/67/52
f 45/63/53 56/68/53 46/56/53
f 42/64/54 53/69/54 43/57/54
f 39/55/55 50/70/55 40/58/55
f 46/56/56 57/71/56 47/59/56
f 44/60/57 53/69/57 54/72/57
f 41/61/58 50/70/58 51/73/58
f 92/74/59 47/59/59 57/71/59
f 44/60/60 55/75/60 45/63/60
f 92/76/61 57/71/61 66/77/61
f 54/72/62 64/78/62 55/75/62
f 51/73/63 61/79/63 52/65/63
f 49/67/64 48/80/64 58/81/64
f 55/75/65 65/82/65 56/68/65
f 52/65/66 62/83/66 53/69/66
f 49/67/67 59/84/67 50/70/67
f 56/68/68 66/77/68 57/71/68
f 54/72/69 62/83/69 63/85/69
f 51/73/70 59/84/70 60/86/70
f 63/87/71 71/88/71 72/89/71
f 60/90/72 68/91/72 69/92/72
f 92/93/73 66/94/73 75/95/73
f 63/87/74 73/96/74 64/97/74
f 60/90/75 70/98/75 61/99/75
f 58/100/76 48/101/76 67/102/76
f 64/97/77 74/103/77 65/104/77
f 61/99/78 71/88/78 62/105/78
f 58/100/79 68/91/79 59/106/79
f 65/104/80 75/95/80 66/94/80
f 67/102/81 77/107/81 68/91/81
f 75/95/82 83/108/82 84/109/82
f 72/89/83 80/110/83 81/111/83
f 69/92/84 77/107/84 78/112/84
f 92/113/85 75/95/85 84/109/85
f 72/89/86 82/114/86 73/96/86
f 69/92/87 79/115/87 70/98/87
f 67/102/88 48/116/88 76/117/88
f 73/96/89 83/108/89 74/103/89
f 71/88/90 79/115/90 80/110/90
f 79/115/91 89/19/91 80/110/91
f 76/117/92 86/4/92 77/107/92
f 83/108/93 91/8/93 84/109/93
f 81/111/94 89/19/94 1/1/94
f 78/112/95 86/4/95 87/6/95
f 92/118/96 84/109/96 91/8/96
f 81/111/97 90/11/97 82/114/97
f 78/112/98 88/13/98 79/115/98
f 76/117/99 48/119/99 85/14/99
f 83/108/100 90/11/100 2/17/100
f 1/1/1 89/19/1 7/2/1
f 86/4/2 4/20/2 5/5/2
f 1/1/4 8/3/4 9/10/4
f 87/6/5 5/5/5 6/12/5
f 2/17/7 90/11/7 9/10/7
f 88/13/8 6/12/8 7/2/8
f 85/14/9 3/16/9 4/20/9
f 91/8/10 2/17/10 10/18/10
f 4/20/11 13/30/11 14/21/11
f 8/3/13 17/31/13 18/24/13
f 6/12/14 5/5/14 14/21/14
f 9/10/16 18/24/16 19/28/16
f 6/12/17 15/25/17 16/29/17
f 3/16/18 12/27/18 13/30/18
f 10/18/19 19/28/19 20/23/19
f 8/3/20 7/2/20 16/29/20
f 20/23/21 19/28/21 28/32/21
f 17/31/22 16/29/22 25/34/22
f 13/30/23 22/42/23 23/36/23
f 17/31/25 26/35/25 27/38/25
f 14/21/26 23/36/26 24/39/26
f 18/24/28 27/38/28 28/32/28
f 15/25/29 24/39/29 25/34/29
f 12/27/30 21/41/30 22/42/30
f 25/34/31 24/39/31 33/43/31
f 21/41/32 30/52/32 31/45/32
f 28/32/33 37/53/33 38/46/33
f 26/35/34 25/34/34 34/44/34
f 23/36/35 22/42/35 31/45/35
f 26/35/37 35/47/37 36/50/37
f 23/36/38 32/48/38 33/43/38
f 27/38/40 36/50/40 37/53/40
f 36/50/42 45/63/42 46/56/42
f 33/43/43 42/64/43 43/57/43
f 30/52/44 39/55/44 40/58/44
f 37/53/45 46/56/45 47/59/45
f 35/47/46 34/44/46 43/57/46
f 32/48/47 31/45/47 40/58/47
f 35/47/49 44/60/49 45/63/49
f 32/48/50 41/61/50 42/64/50
f 41/61/51 51/73/51 52/65/51
f 45/63/53 55/75/53 56/68/53
f 42/64/54 52/65/54 53/69/54
f 39/55/55 49/67/55 50/70/55
f 46/56/56 56/68/56 57/71/56
f 44/60/57 43/57/57 53/69/57
f 41/61/58 40/58/58 50/70/58
f 44/60/60 54/72/60 55/75/60
f 54/72/62 63/85/62 64/78/62
f 51/73/63 60/86/63 61/79/63
f 55/75/65 64/78/65 65/82/65
f 52/65/66 61/79/66 62/83/66
f 49/67/67 58/81/67 59/84/67
f 56/68/68 65/82/68 66/77/68
f 54/72/69 53/69/69 62/83/69
f 51/73/70 50/70/70 59/84/70
f 63/87/71 62/105/71 71/88/71
f 60/90/72 59/106/72 68/91/72
f 63/87/74 72/89/74 73/96/74
f 60/90/75 69/92/75 70/98/75
f 64/97/77 73/96/77 74/103/77
f 61/99/78 70/98/78 71/88/78
f 58/100/79 67/102/79 68/91/79
f 65/104/80 74/103/80 75/95/80
f 67/102/81 76/117/81 77/107/81
f 75/95/82 74/103/82 83/108/82
f 72/89/83 71/88/83 80/110/83
f 69/92/84 68/91/84 77/107/84
f 72/89/86 81/111/86 82/114/86
f 69/92/87 78/112/87 79/115/87
f 73/96/89 82/114/89 83/108/89
f 71/88/90 70/98/90 79/115/90
f 79/115/91 88/13/91 89/19/91
f 76/117/92 85/14/92 86/4/92
f 83/108/93 2/17/93 91/8/93
f 81/111/94 80/110/94 89/19/94
f 78/112/95 77/107/95 86/4/95
f 81/111/97 1/1/97 90/11/97
f 78/112/98 87/6/98 88/13/98
f 83/108/100 82/114/100 90/11/100
)";

	LoadObj();
}

Sphere::~Sphere() {

}

Arrow::Arrow() {

	rawData = R"(
o Cone
v 0.000000 0.800000 -0.100000
v 0.070711 0.800000 -0.070711
v 0.100000 0.800000 -0.000000
v 0.000000 1.000000 0.000000
v 0.070711 0.800000 0.070711
v -0.000000 0.800000 0.100000
v -0.070711 0.800000 0.070711
v -0.100000 0.800000 -0.000000
v -0.070711 0.800000 -0.070711
s off
f 4 7 6
f 5 7 2
f 4 8 7
f 3 4 5
f 5 4 6
f 4 9 8
f 4 1 9
f 2 1 4
f 2 4 3
f 9 1 2
f 2 3 5
f 5 6 7
f 7 8 9
f 9 2 7
o Cylinder
v 0.000000 0.000000 -0.050000
v 0.009755 0.900000 -0.049039
v 0.019134 0.000000 -0.046194
v 0.027779 0.900000 -0.041573
v 0.035355 0.000000 -0.035355
v 0.041573 0.900000 -0.027779
v 0.046194 0.000000 -0.019134
v 0.049039 0.900000 -0.009755
v 0.050000 0.000000 -0.000000
v 0.049039 0.900000 0.009755
v 0.046194 0.000000 0.019134
v 0.041573 0.900000 0.027779
v 0.035355 0.000000 0.035355
v 0.027779 0.900000 0.041573
v 0.019134 0.000000 0.046194
v 0.009755 0.900000 0.049039
v -0.000000 0.000000 0.050000
v -0.009755 0.900000 0.049039
v -0.019134 0.000000 0.046194
v -0.027779 0.900000 0.041573
v -0.035355 0.000000 0.035355
v -0.041574 0.900000 0.027778
v -0.046194 0.000000 0.019134
v -0.049039 0.900000 0.009754
v -0.050000 0.000000 -0.000000
v -0.049039 0.900000 -0.009755
v -0.046194 0.000000 -0.019134
v -0.041573 0.900000 -0.027779
v -0.035355 0.000000 -0.035355
v -0.027778 0.900000 -0.041574
v -0.019134 0.000000 -0.046194
v -0.009754 0.900000 -0.049039
s off
f 13 15 14
f 16 14 15
f 17 19 18
f 18 16 17
f 19 21 20
f 20 18 19
f 21 23 22
f 22 20 21
f 23 25 24
f 24 22 23
f 25 27 26
f 26 24 25
f 27 29 28
f 28 26 27
f 29 31 30
f 30 28 29
f 31 33 32
f 32 30 31
f 33 35 34
f 34 32 33
f 35 37 36
f 36 34 35
f 37 39 38
f 38 36 37
f 41 40 39
f 40 38 39
f 41 10 40
f 29 21 37
f 11 12 10
f 24 32 16
f 15 17 16
f 11 13 12
f 14 12 13
f 10 41 11
f 13 11 41
f 41 39 37
f 37 35 33
f 33 31 29
f 29 27 25
f 25 23 29
f 21 19 17
f 17 15 13
f 13 41 37
f 37 33 29
f 29 23 21
f 21 17 13
f 13 37 21
f 40 10 12
f 12 14 16
f 16 18 20
f 20 22 24
f 24 26 28
f 28 30 32
f 32 34 36
f 36 38 40
f 40 12 16
f 16 20 24
f 24 28 32
f 32 36 40
f 40 16 32
)";

	LoadObj();
}

Arrow::~Arrow() {

}

Cylinder::Cylinder() {

	rawData = R"(
o Cylinder
v 0.000000 -0.500000 -0.500000
v 0.000000 0.500000 -0.500000
v 0.293893 -0.500000 -0.404509
v 0.293893 0.500000 -0.404509
v 0.475528 -0.500000 -0.154508
v 0.475528 0.500000 -0.154508
v 0.475528 -0.500000 0.154509
v 0.475528 0.500000 0.154509
v 0.293893 -0.500000 0.404509
v 0.293893 0.500000 0.404509
v -0.000000 -0.500000 0.500000
v -0.000000 0.500000 0.500000
v -0.293893 -0.500000 0.404508
v -0.293893 0.500000 0.404508
v -0.475528 -0.500000 0.154509
v -0.475528 0.500000 0.154509
v -0.475528 -0.500000 -0.154509
v -0.475528 0.500000 -0.154509
v -0.293892 -0.500000 -0.404509
v -0.293892 0.500000 -0.404509
vt 1.000000 1.000000
vt 0.900000 0.500000
vt 1.000000 0.500000
vt 0.900000 1.000000
vt 0.800000 0.500000
vt 0.800000 1.000000
vt 0.700000 0.500000
vt 0.700000 1.000000
vt 0.600000 0.500000
vt 0.600000 1.000000
vt 0.500000 0.500000
vt 0.500000 1.000000
vt 0.400000 0.500000
vt 0.400000 1.000000
vt 0.300000 0.500000
vt 0.300000 1.000000
vt 0.200000 0.500000
vt 0.250000 0.490000
vt 0.021746 0.324164
vt 0.391068 0.055836
vt 0.200000 1.000000
vt 0.100000 0.500000
vt 0.100000 1.000000
vt -0.000000 0.500000
vt 0.978254 0.175836
vt 0.750000 0.010000
vt 0.521746 0.175836
vt 0.478254 0.324164
vt 0.391068 0.444164
vt 0.108932 0.444164
vt 0.021746 0.175836
vt 0.108932 0.055836
vt 0.250000 0.010000
vt 0.478254 0.175836
vt -0.000000 1.000000
vt 0.608932 0.444164
vt 0.750000 0.490000
vt 0.891068 0.444164
vt 0.978254 0.324164
vt 0.891068 0.055836
vt 0.608932 0.055836
vt 0.521746 0.324164
vn 0.3090 0.0000 -0.9511
vn 0.8090 0.0000 -0.5878
vn 1.0000 0.0000 0.0000
vn 0.8090 0.0000 0.5878
vn 0.3090 0.0000 0.9511
vn -0.3090 0.0000 0.9511
vn -0.8090 0.0000 0.5878
vn -1.0000 0.0000 0.0000
vn 0.0000 1.0000 0.0000
vn -0.8090 0.0000 -0.5878
vn -0.3090 0.0000 -0.9511
vn 0.0000 -1.0000 0.0000
usemtl None
s off
f 2/1/1 3/2/1 1/3/1
f 4/4/2 5/5/2 3/2/2
f 6/6/3 7/7/3 5/5/3
f 8/8/4 9/9/4 7/7/4
f 10/10/5 11/11/5 9/9/5
f 12/12/6 13/13/6 11/11/6
f 14/14/7 15/15/7 13/13/7
f 16/16/8 17/17/8 15/15/8
f 2/18/9 18/19/9 10/20/9
f 18/21/10 19/22/10 17/17/10
f 20/23/11 1/24/11 19/22/11
f 7/25/12 11/26/12 15/27/12
f 2/1/1 4/4/1 3/2/1
f 4/4/2 6/6/2 5/5/2
f 6/6/3 8/8/3 7/7/3
f 8/8/4 10/10/4 9/9/4
f 10/10/5 12/12/5 11/11/5
f 12/12/6 14/14/6 13/13/6
f 14/14/7 16/16/7 15/15/7
f 16/16/8 18/21/8 17/17/8
f 6/28/9 4/29/9 2/18/9
f 2/18/9 20/30/9 18/19/9
f 18/19/9 16/31/9 14/32/9
f 14/32/9 12/33/9 18/19/9
f 12/33/9 10/20/9 18/19/9
f 10/20/9 8/34/9 6/28/9
f 6/28/9 2/18/9 10/20/9
f 18/21/10 20/23/10 19/22/10
f 20/23/11 2/35/11 1/24/11
f 19/36/12 1/37/12 3/38/12
f 3/38/12 5/39/12 7/25/12
f 7/25/12 9/40/12 11/26/12
f 11/26/12 13/41/12 15/27/12
f 15/27/12 17/42/12 19/36/12
f 19/36/12 3/38/12 15/27/12
f 3/38/12 7/25/12 15/27/12
)";


	LoadObj();
}

Cylinder::~Cylinder() {

}


Line::Line() {

	rawData = R"(
o line
v 0.000000 0.000100 -0.500000
v 0.000000 0.000100 0.500000
v 0.000087 -0.000050 -0.500000
v 0.000087 -0.000050 0.500000
v -0.000087 -0.000050 -0.500000
v -0.000087 -0.000050 0.500000
vt 1.000000 1.000000
vt 0.666667 0.500000
vt 1.000000 0.500000
vt 0.666667 1.000000
vt 0.333333 0.500000
vt 0.457846 0.130000
vt 0.250000 0.490000
vt 0.042154 0.130000
vt 0.333333 1.000000
vt -0.000000 0.500000
vt 0.750000 0.490000
vt 0.957846 0.130000
vt 0.542154 0.130000
vt -0.000000 1.000000
vn 0.8660 0.5000 0.0000
vn -0.0000 -1.0000 -0.0000
vn 0.0000 0.0000 1.0000
vn -0.8660 0.5000 0.0000
vn 0.0000 0.0000 -1.0000
usemtl None
s off
f 2/1/1 3/2/1 1/3/1
f 4/4/2 5/5/2 3/2/2
f 4/6/3 2/7/3 6/8/3
f 6/9/4 1/10/4 5/5/4
f 1/11/5 3/12/5 5/13/5
f 2/1/1 4/4/1 3/2/1
f 4/4/2 6/9/2 5/5/2
f 6/9/4 2/14/4 1/10/4
)";


	LoadObj();
}

Line::~Line() {

}