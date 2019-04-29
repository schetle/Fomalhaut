#pragma once

#include <map>
#include "component_pool.h"
#include "ecs_functional.h"

namespace ecs {
	class ComponentPool;
	class Entity {
	private:
		ComponentPool& pool_;
		EntityID eid_;
		std::map<index_t, BaseComponent*> components_;

	public:
		Entity(ComponentPool& pool, EntityID eid);
		virtual ~Entity();

		EntityID GetEntityID() const { return eid_; }

		template <typename T, typename... TArgs>
		auto Add(TArgs&&... args)->Entity&;

		template <typename Arg>
		auto Remove()->Entity&;
		template <typename Arg0, typename... Args>
		auto Remove() -> typename std::enable_if<sizeof...(Args) != 0, Entity&>::type;

		template <typename T, typename... TArgs>
		auto Replace(TArgs&&... args)->Entity&;

		template <typename T>
		auto Get() const->T*;
		template <typename... Args>
		auto Get() const -> typename std::enable_if<(sizeof...(Args) != 1), std::tuple<Args*...> >::type;

		template <typename Arg>
		bool Has() const;
		template <typename Arg0, typename... Args>
		auto Has() const -> typename std::enable_if<sizeof...(Args) != 0, bool>::type;

		BaseComponent* GetComponent(const index_t index) const;
        const std::map<index_t, BaseComponent*>& GetAllComponents() const { return components_; }

	private:
		Entity& AddComponent(const index_t index, BaseComponent* component);
		Entity& RemoveComponent(const index_t index);
		Entity& ReplaceComponent(const index_t index, BaseComponent* component);
		bool HasComponent(const index_t index) const;
		void Destroy();
		void ReplaceWith(const index_t index, BaseComponent* replacement);

		void DestroyAllComponents();
	};

	template <typename T, typename... TArgs>
	auto Entity::Add(TArgs&&... args) -> Entity& {
		return AddComponent(details::ComponentIndex::index<T>(),
			pool_.CreateComponent<T>(std::forward<TArgs>(args)...));
	}

	template <typename Arg>
	auto Entity::Remove() -> Entity& {
		return RemoveComponent(details::ComponentIndex::index<Arg>());
	}
	template <typename Arg0, typename... Args>
	auto Entity::Remove() ->
		typename std::enable_if<sizeof...(Args) != 0, Entity&>::type {
		return RemoveComponent(details::ComponentIndex::index<Arg0>()),
			Remove<Args...>();
	}

	template <typename T, typename... TArgs>
	auto Entity::Replace(TArgs&&... args) -> Entity& {
		return ReplaceComponent(
			details::ComponentIndex::index<T>(),
			pool_.CreateComponent<T>(std::forward<TArgs>(args)...));
	}

	template <typename T>
	auto Entity::Get() const -> T* {
		return static_cast<T*>(GetComponent(details::ComponentIndex::index<T>()));
	}

	template<typename ...Args>
	auto ecs::Entity::Get() const -> typename std::enable_if<(sizeof ...(Args) != 1), std::tuple<Args *...>>::type
	{
		using ValueType = std::tuple<Args*...>;
		if (Has<Args...>()) {
			return ValueType(Get<Args>()...);
		}
		return ValueType();
	}

	template <typename Arg>
	bool Entity::Has() const {
		return HasComponent(details::ComponentIndex::index<Arg>());
	}

	template <typename Arg0, typename... Args>
	auto Entity::Has() const ->
		typename std::enable_if<sizeof...(Args) != 0, bool>::type {
		return HasComponent(details::ComponentIndex::index<Arg0>()) && Has<Args...>();
	}

}  // namespace ecs