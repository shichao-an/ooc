// new(Point, x, y)

% PointClass: Class  Point: Object {	// header
    int x;								// object components
    int y;
%										// statically linked
    void move (_self, int dx, int dy);
%-										// dynamically linked
    void draw (const _self);
%}

%prot

#define	x(p)	(((const struct Point *)(p)) -> x)
#define	y(p)	(((const struct Point *)(p)) -> y)
