/*
	Copyright (C) 2003-2013 by David White <davewx7@gmail.com>
	
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef REFERENCE_COUNTED_OBJECT_HPP_INCLUDED
#define REFERENCE_COUNTED_OBJECT_HPP_INCLUDED

#include <assert.h>

#include "boost/intrusive_ptr.hpp"

class reference_counted_object
{
public:
	reference_counted_object() : count_(0) {}
	reference_counted_object(const reference_counted_object& /*obj*/) : count_(0) {}
	reference_counted_object& operator=(const reference_counted_object& /*obj*/) {
		return *this;
	}
	virtual ~reference_counted_object() { }

	void add_ref() const { ++count_; }
	void dec_ref() const { if(--count_ == 0) { delete this; } }
	void dec_ref_norelease() const { --count_; }

	int refcount() const { return count_; }

protected:
	void turn_reference_counting_off() { count_ = 1000000; }
private:
	mutable int count_;
};

struct reference_counted_object_pin_norelease
{
	reference_counted_object* obj_;
	reference_counted_object_pin_norelease(reference_counted_object* obj) : obj_(obj)
	{
		obj_->add_ref();
	}
	~reference_counted_object_pin_norelease()
	{
		obj_->dec_ref_norelease();
	}
};

inline void intrusive_ptr_add_ref(const reference_counted_object* obj) {
	obj->add_ref();
}

inline void intrusive_ptr_release(const reference_counted_object* obj) {
	obj->dec_ref();
}

typedef boost::intrusive_ptr<reference_counted_object> object_ptr;
typedef boost::intrusive_ptr<const reference_counted_object> const_object_ptr;

#endif
