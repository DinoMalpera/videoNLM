#pragma once

/*	Standard NonCopyable mixin.
 * */
class NonCopyable
{
public: 
	NonCopyable ( const NonCopyable& ) = delete;
public:
	NonCopyable& operator=( const NonCopyable& ) = delete;
protected:
	NonCopyable() = default;
};

