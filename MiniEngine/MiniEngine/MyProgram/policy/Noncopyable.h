#pragma once
/// <summary>
/// NoncopyÉ|ÉäÉVÅ[
/// </summary>

#ifndef _ENGINENONCOPYABLE_H_
#define _ENGINENONCOPYABLE_H_

namespace Engine {
	struct Noncopyable {
		Noncopyable() = default;
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};
}

#endif	//_ENGINENONCOPYABLE_H_