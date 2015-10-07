
// Write any additional support functions as well as the actual kernel function in this file

struct Color{
	float angle;
	float intensity;
};

struct CircleInfo{
	float x;
	float y;
	float radius;
	struct Color color;
};

struct LineInfo{
	float x1,y1;
	float x2,y2;
	float thickness;
	struct Color color;
};

float red( float deg ) {
	float a1 = 1.f/60;
	float b1 = 2;
	float a2 = -1.f/60;
	float b2 = 2;
	float asc = deg*a2+b2;
	float desc = deg*a1+b1;
	return fmax( .0f , fmin( 1.f, fmin(asc,desc)));
}

float green( float deg ) {
	float a1 = 1.f/60;
	float b1 = 0;
	float a2 = -1.f/60;
	float b2 = 4;
	float asc = deg*a2+b2;
	float desc = deg*a1+b1;
	return fmax( .0f , fmin( 1.f, fmin(asc,desc)));
}

float blue( float deg ) {
	float a1 = 1.f/60;
	float b1 = -2;
	float a2 = -1.f/60;
	float b2 = 6;
	float asc = deg*a2+b2;
	float desc = deg*a1+b1;
	return fmax( .0f , fmin( 1.f, fmin(asc,desc)));
}

/* On CMB, this function needs to be redeclared with the below modifier:
struct vec3 __OVERLOADABLE__ cross()*/
struct vec3 cross( struct vec3 a,struct vec3 b){
	struct vec3 ret ;
	ret.x = a.y*b.z - a.z*b.y;
	ret.y = a.z*b.x - a.x*b.z;
	ret.z = a.x*b.y - a.y*b.x;
	return ret;
}


/* On CMB, this function needs to be redeclared with the below modifier:
float __OVERLOADABLE__ dot()*/
float dot( struct vec3 a , struct vec3 b ){
	return a.x*b.x+ a.y*b.y+ a.z*b.z;
}

int max(float a, float b) {
	if (a > b) {
		return a;
	}
	return b;
}

int min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}

int isInBounds(float xmin, float ymin, float xmax, float ymax, float x, float y) {
	if ((x >= xmin && x <= xmax) && (y >= ymin && y <= ymax)) {
		return 1;
	}
	return 0;
}

int isPixelInCircle(circ_x, circ_y, r, x, y) {
	float dy = circ_y - y;
	float dx = circ_x - x;
	float d = sqrt(pow(dx, 2.0) + pow(dy, 2.0));
	if (d <= r)
		return 1;
	return 0;
}

int isPixelOnLine(x1, y1, x2, y2, thickness, x, y) {
	float deltaX = x2 - x1;
	float deltaY = y2 - y1;
	float ax = deltaY / deltaX;
	float b = (x2 * y1 - x1 * y2) / deltaX;

	float xmin = min(x1, x2);
	float xmax = max(x1, x2);
	float ymin = min(y1, y2);
	float ymax = max(y1, y2);

	if ((fabs(y - (ax * x + b)) < thickness * 0.5) && isInBounds(xmin, ymin, xmax, ymax, x, y)) {
		return 1;
	}
	return 0;
}
