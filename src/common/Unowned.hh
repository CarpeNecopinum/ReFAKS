#pragma once

template<class T>
class Unowned {
public:
    template<class Owned>
    Unowned(Owned&&) {
        static_assert(sizeof(Owned) != sizeof(Owned),
                      "Don't move things into Unowned!");
    }

    template<class Owned>
    Unowned(Owned const& owned)
        : mPtr(owned.get())
    {}

    T* operator->() { return mPtr; }
    T& operator*() { return &mPtr; }
    T* get() { return mPtr; }

protected:
    const T* mPtr;
};
