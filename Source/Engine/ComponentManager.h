#pragma once
#include "Component.h"
#include "FileResource.h"
#include "Types.h"
#include "vec3.h"

namespace Plasmium {
    class ComponentManager {
    public:
        template<typename... Args>
        void CreateComponentBase(const ComponentCreationArgs& creationArgs, Args&&... args) {
            CreateComponent(creationArgs, args...);
        }

        virtual void CreateComponent(const ComponentCreationArgs& creationArgs)
        { }

        virtual void CreateComponent(const ComponentCreationArgs& creationArgs,
            const FileResource& a)
        { }

        virtual void CreateComponent(const ComponentCreationArgs& creationArgs,
            const FileResource& a,
            const FileResource& b)
        { }

        virtual void CreateComponent(const ComponentCreationArgs & creationArgs,
            const vec3& a,
            const vec3& b,
            const vec3& c)
        { }

        virtual void CreateComponent(const ComponentCreationArgs & creationArgs,
            const vec3 & a,
            const vec3 & b,
            const vec3 & c,
            const vec3& d)
        { }

        virtual void DeleteComponent(EntityId id, ComponentType type) = 0;
    };
}