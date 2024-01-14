/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.hpp
*/

#pragma once

#include "../Coordinator.hpp"
#include "../System.hpp"
#include "../components/Physics.hpp"
#include "../../core/event/CollisionEvent.hpp"

namespace ecs {
    namespace system {
        static constexpr unsigned int cMaxBodies = 1024;
        static constexpr unsigned int cNumBodyMutexes = 0;
        static constexpr unsigned int cMaxBodyPairs = 1024;
        static constexpr unsigned int cMaxContactConstraints = 1024;

        namespace Layers
        {
            static constexpr JPH::ObjectLayer NON_MOVING = 0;
            static constexpr JPH::ObjectLayer MOVING = 1;
            static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
        };

        namespace BroadPhaseLayers
        {
            static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
            static constexpr JPH::BroadPhaseLayer MOVING(1);
            static constexpr unsigned int NUM_LAYERS(2);
        };

        #define JPH_EXTERNAL_PROFILEtween object and broadphase layers.
        class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
            public:
                BPLayerInterfaceImpl() {
                    // Create a mapping table from object to broad phase layer
                    mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
                    mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
                }

                virtual unsigned int GetNumBroadPhaseLayers() const override {
                    return BroadPhaseLayers::NUM_LAYERS;
                }

                virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
                    JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
                    return mObjectToBroadPhase[inLayer];
                }

            #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
                virtual const char *GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override {
                    switch ((JPH::BroadPhaseLayer::Type)inLayer) {
                        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
                        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
                        default:													JPH_ASSERT(false); return "INVALID";
                    }
                }
            #endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

            private:
                JPH::BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
        };

        class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
            public:
                virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override {
                    switch (inLayer1) {
                        case Layers::NON_MOVING:
                            return inLayer2 == BroadPhaseLayers::MOVING;
                        case Layers::MOVING:
                            return true;
                        default:
                            JPH_ASSERT(false);
                            return false;
                    }
                }
        };

        class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
            public:
                virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override {
                    switch (inObject1) {
                        case Layers::NON_MOVING:
                            return inObject2 == Layers::MOVING; // Non moving only collides with moving
                        case Layers::MOVING:
                            return true; // Moving collides with everything
                        default:
                            JPH_ASSERT(false);
                            return false;
                    }
                }
        };

        class MyContactListener : public JPH::ContactListener {
            public:
                // See: ContactListener
                virtual JPH::ValidateResult	OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) override
                {
                    //std::cout << "Contact validate callback" << std::endl;

                    // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
                    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
                }

                virtual void OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
                {
                    //std::cout << "A contact was added" << std::endl;
                }

                virtual void OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
                {
                    //std::cout << "A contact was persisted" << std::endl;
                }

                virtual void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override
                {
                    //std::cout << "A contact was removed" << std::endl;
                }
        };

        class JoltPhysicsSystem : public System {
            public:
                void update(JPH::PhysicsSystem &joltPhysicsSystem, float timeStep);
        };
    }
}