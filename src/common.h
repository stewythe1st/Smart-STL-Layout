#ifndef COMMON_H
#define COMMON_H

#define MAX2(a, b)			(std::max(a, b))
#define MIN2(a, b)			(std::min(a, b))
#define MAX3(a, b, c)		(MAX2(a, MAX2(b, c)))
#define MIN3(a, b, c)		(MIN2(a, MIN2(b, c)))
#define MAX4(a, b, c, d)	(MAX2(MAX2(a, b), MAX2(c, d)))
#define MIN4(a, b, c, d)	(MIN2(MIN2(a, b), MIN2(c, d)))

#define GET_RANDOM_DECIMAL	((float)(rand() % 1000) / 1000.0f)

#define FORMAT_FLOAT(x)		std::fixed << std::setprecision(x) << std::setfill('0')

#define PI					3.14159f
#define DEG_2_RAD(deg)		(deg * PI / 180.0f)

#endif
