#pragma once
#include "Array.h"
#include "HashTable.h"
#include "Types.h"

template <typename Handle, typename Object>
class Handler {
private:
    HashTable<Handle, uint32> lookup;
    Array<Object> objects;
public:
    bool Contains(Handle id) { return lookup.Contains(id); }
    Object* CreateObject(Handle id);
    Object* EmplaceObject(Handle id, const Object& object);
    Object* EmplaceObject(Handle id, Object&& object);
    Object* GetObjectPtr(Handle id);
    void DeleteObject(Handle id);

    const HashTable<Handle, uint32>& GetLookupReference() { return lookup; }
    const Array<Object>& GetObjectsReference() { return objects; }
};

template <typename Handle, typename Object>
Object* Handler<Handle, Object>::CreateObject(Handle id)
{
    objects.Push(Object());
    uint32 arrayIndex = objects.Size() - 1;
    lookup[id] = arrayIndex;

    return &objects[arrayIndex];
}

template <typename Handle, typename Object>
Object* Handler<Handle, Object>::EmplaceObject(Handle id, const Object& object)
{
    objects.Push(object);
    uint32 arrayIndex = objects.Size() - 1;
    lookup[id] = arrayIndex;

    return &objects[arrayIndex];
}

template <typename Handle, typename Object>
Object* Handler<Handle, Object>::EmplaceObject(Handle id, Object&& object)
{
    objects.Push(std::move(object));
    uint32 arrayIndex = objects.Size() - 1;
    lookup[id] = arrayIndex;

    return &objects[arrayIndex];
}

template <typename Handle, typename Object>
Object* Handler<Handle, Object>::GetObjectPtr(Handle id)
{
    if (!lookup.Contains(id)) {
        return nullptr;
    }

    return &objects[lookup[id]];
}

template <typename Handle, typename Object>
void Handler<Handle, Object>::DeleteObject(Handle id)
{
    if (!lookup.Contains(id)) {
        return;
    }

    // Remove entity
    uint32 arrayIndex = lookup[id];
    lookup.Delete(id);
    objects.Delete(arrayIndex);

    if (arrayIndex < objects.Size()) {
        // Fix up replacing entity
        Handle replacingId = objects[arrayIndex].GetId();
        lookup[replacingId] = arrayIndex;
    }
}