#pragma once

#include "error_macros.hpp"

template<typename TElement, typename TAllocator = std::allocator<TElement>>
class LocalVector {
	using Implementation = std::vector<TElement, TAllocator>;

public:
	using Iterator = typename Implementation::iterator;
	using ConstIterator = typename Implementation::const_iterator;

	LocalVector() = default;

	explicit LocalVector(int32_t p_capacity) { storage.reserve(p_capacity); }

	LocalVector(std::initializer_list<TElement> p_list)
		: storage(p_list) { }

	_FORCE_INLINE_ void push_back(const TElement& p_value) { emplace_back(p_value); }

	_FORCE_INLINE_ void push_back(TElement&& p_value) { emplace_back(std::move(p_value)); }

	template<typename... TArgs>
	_FORCE_INLINE_ void emplace_back(TArgs&&... p_args) {
		storage.emplace_back(std::forward<TArgs>(p_args)...);
	}

	_FORCE_INLINE_ void remove_at(int32_t p_index) {
		ERR_FAIL_INDEX(p_index, size());

		storage.erase(begin() + p_index);
	}

	_FORCE_INLINE_ void remove_at_unordered(int32_t p_index) {
		ERR_FAIL_INDEX(p_index, size());

		const int32_t last_index = size() - 1;

		auto element_iter = begin() + p_index;
		auto last_element_iter = begin() + last_index;

		std::swap(*element_iter, *last_element_iter);

		storage.erase(last_element_iter);
	}

	_FORCE_INLINE_ void erase(const TElement& p_value) {
		auto new_end = std::remove(begin(), end(), p_value);
		if (new_end != end()) {
			storage.erase(new_end, end());
		}
	}

	template<typename TCallable>
	_FORCE_INLINE_ void erase_if(TCallable&& p_callable) {
		auto new_end = std::remove_if(begin(), end(), std::forward<TCallable>(p_callable));
		if (new_end != end()) {
			storage.erase(new_end, end());
		}
	}

	_FORCE_INLINE_ void invert() { std::reverse(begin(), end()); }

	_FORCE_INLINE_ void clear() { storage.clear(); }

	_FORCE_INLINE_ bool is_empty() const { return storage.empty(); }

	_FORCE_INLINE_ int32_t get_capacity() const { return (int32_t)storage.capacity(); }

	_FORCE_INLINE_ void reserve(int32_t p_capacity) { storage.reserve((size_t)p_capacity); }

	_FORCE_INLINE_ int32_t size() const { return (int32_t)storage.size(); }

	_FORCE_INLINE_ void resize(int32_t p_size) { storage.resize((size_t)p_size); }

	_FORCE_INLINE_ void insert(int32_t p_index, const TElement& p_value) {
		emplace(p_index, p_value);
	}

	_FORCE_INLINE_ void insert(int32_t p_index, TElement&& p_value) {
		emplace(p_index, std::move(p_value));
	}

	template<typename... TArgs>
	_FORCE_INLINE_ void emplace(int32_t p_index, TArgs&&... p_args) {
		ERR_FAIL_INDEX(p_index, size() + 1);

		storage.emplace(begin() + p_index, std::forward<TArgs>(p_args)...);
	}

	_FORCE_INLINE_ void ordered_insert(const TElement& p_val) {
		auto position = std::lower_bound(begin(), end(), p_val);
		storage.insert(position, p_val);
	}

	_FORCE_INLINE_ void ordered_insert(TElement&& p_val) {
		auto position = std::lower_bound(begin(), end(), p_val);
		storage.insert(position, std::move(p_val));
	}

	_FORCE_INLINE_ int32_t find(const TElement& p_value, int32_t p_from = 0) const {
		if (p_from < size()) {
			auto found = std::find(begin() + p_from, end(), p_value);
			if (found != end()) {
				return (int32_t)std::distance(begin(), found);
			}
		}

		return -1;
	}

	template<typename TCallable>
	_FORCE_INLINE_ int32_t find_if(TCallable&& p_callable, int32_t p_from = 0) const {
		if (p_from < size()) {
			auto found = std::find_if(begin() + p_from, end(), std::forward<TCallable>(p_callable));
			if (found != end()) {
				return (int32_t)std::distance(begin(), found);
			}
		}

		return -1;
	}

	_FORCE_INLINE_ void sort() { std::sort(begin(), end()); }

	_FORCE_INLINE_ TElement* ptr() { return storage.data(); }

	_FORCE_INLINE_ const TElement* ptr() const { return storage.data(); }

	_FORCE_INLINE_ Iterator begin() { return storage.begin(); }

	_FORCE_INLINE_ Iterator end() { return storage.end(); }

	_FORCE_INLINE_ ConstIterator begin() const { return storage.begin(); }

	_FORCE_INLINE_ ConstIterator end() const { return storage.end(); }

	_FORCE_INLINE_ ConstIterator cbegin() const { return storage.cbegin(); }

	_FORCE_INLINE_ ConstIterator cend() const { return storage.cend(); }

	_FORCE_INLINE_ TElement& operator[](int32_t p_index) { return storage[(size_t)p_index]; }

	_FORCE_INLINE_ const TElement& operator[](int32_t p_index) const {
		return storage[(size_t)p_index];
	}

	_FORCE_INLINE_ LocalVector& operator=(std::initializer_list<TElement> p_list) {
		storage = p_list;
		return *this;
	}

private:
	Implementation storage;
};
