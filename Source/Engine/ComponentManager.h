#pragma once
#include "Component.h"
#include "FileResource.h"
#include "Types.h"
#include "vec3.h"
#include "Window.h"

namespace Plasmium {
    class ComponentManager {
    public:
        template<typename... Args>
        void CreateComponentBase(const ComponentCreationArgs& creationArgs, Args&&... args)
        {
            CreateComponent(creationArgs, args...);
        }

        virtual void CreateComponent(const ComponentCreationArgs& creationArgs)
        {
            Window::WriteError("Unhandled CreateComponent");
        }

        virtual void CreateComponent(const ComponentCreationArgs& creationArgs,
            const char* a)
        {
            Window::WriteError("Unhandled CreateComponent");
        }

        virtual void CreateComponent(const ComponentCreationArgs& creationArgs,
            const FileResource& a)
        {
            Window::WriteError("Unhandled CreateComponent");
        }

        virtual void CreateComponent(const ComponentCreationArgs& creationArgs,
            const FileResource& a,
            const FileResource& b)
        {
            Window::WriteError("Unhandled CreateComponent");
        }

        virtual void CreateComponent(const ComponentCreationArgs & creationArgs,
            const vec3& a,
            const vec3& b,
            const vec3& c)
        {
            Window::WriteError("Unhandled CreateComponent");
        }

        virtual void CreateComponent(const ComponentCreationArgs & creationArgs,
            const vec3 & a,
            const vec3 & b,
            const vec3 & c,
            const vec3& d)
        {
            Window::WriteError("Unhandled CreateComponent");
        }

        virtual void CreateComponent(const ComponentCreationArgs & creationArgs,
            float a,
            float b)
        {
            Window::WriteError("Unhandled CreateComponent");
        }

        virtual void PreDeleteComponent(EntityId id, ComponentType type) {}
        virtual void DeleteComponent(EntityId id, ComponentType type) = 0;
    };
}