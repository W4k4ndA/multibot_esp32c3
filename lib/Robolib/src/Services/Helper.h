#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <cstddef>
#include "Interfaces/IRobotModule.h"

namespace robolib {

/**
 * @brief Helper de búsqueda genérica de módulos por tipo T (Interfaz).
 */
class ModuleHelper {
protected:
    const std::vector<IRobotModule*>& modules;

public:
    explicit ModuleHelper(const std::vector<IRobotModule*>& mods) : modules(mods) {}

    /**
     * @brief Obtiene el n-ésimo módulo que implemente la interfaz T.
     */
    template <typename T>
    T* getModule(size_t index = 0) const {
        size_t count = 0;
        for (auto* module : modules) {
            T* casted = dynamic_cast<T*>(module);
            if (casted) {
                if (count == index) return casted;
                count++;
            }
        }
        return nullptr;
    }

    /**
     * @brief Cuenta cuántos módulos registrados implementan la interfaz T.
     */
    template <typename T>
    size_t countModules() const {
        size_t count = 0;
        for (auto* module : modules) {
            if (dynamic_cast<T*>(module) != nullptr) {
                count++;
            }
        }
        return count;
    }
};

} // namespace robolib

#endif // MODULE_REGISTRY_H