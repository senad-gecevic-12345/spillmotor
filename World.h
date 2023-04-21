#ifndef WORLD_H_
#define WORLD_H_


#include "Utils.h"
#include "bullet.h"
//#include "File.h"
//#include "Actor.h"
#include <memory>



namespace World {

	class Clock {
	private:
		float dt;
		double init_time;
		double last_update;
	public:
        Clock& operator=(const Clock&)=delete;
        Clock(const Clock&)=delete;
		static Clock& get();
		float get_dt() const;
		float get_time() const;
        float get_time_since_start()const;
		void update();
        Clock();
        ~Clock()=default;
	};
	float get_time();
}


#endif // WORLD_H_
