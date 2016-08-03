#include "quickVtkPerlinNoise.hpp"

namespace quick {

    namespace Vtk {

        Qml::Register::Class<PerlinNoise> PerlinNoise::Register;

        PerlinNoise::PerlinNoise() : ImplicitFunction(vtkSmartPointer<vtkPerlinNoise>::New()) {
            this->m_vtkObject = vtkPerlinNoise::SafeDownCast(ImplicitFunction::getVtkObject());

            this->m_frequencyCb = [this] (Math::Vector3&& vector) {
                this->updateFrequency(std::move(vector));
            };

            this->m_phaseCb = [this] (Math::Vector3&& vector) {
                this->updatePhase(std::move(vector));
            };
        }

        auto PerlinNoise::setAmplitude(float angle) -> void {
            this->m_vtkObject->SetAmplitude(angle);
            emit this->amplitudeChanged();
            this->update();
        }

        auto PerlinNoise::getAmplitude() -> float {
            return this->m_amplitude;
        }

        auto PerlinNoise::updateFrequency(Math::Vector3&& vector) -> void {
            this->m_vtkObject->SetFrequency(vector.getValues().data());
            this->update();
        }

        auto PerlinNoise::updatePhase(Math::Vector3&& vector) -> void {
            this->m_vtkObject->SetPhase(vector.getValues().data());
            this->update();
        }

        auto PerlinNoise::setFrequency(Math::Vector3* vector) -> void {
            if (this->m_frequency) {
                this->m_frequency->removeCallback(std::move(this->m_frequencyCb));
            }

            this->m_frequency = vector;

            if (vector) {
                vector->addCallback(std::move(this->m_frequencyCb));
                this->updateFrequency(std::move(*vector));
            }

            emit this->frequencyChanged();
        }

        auto PerlinNoise::getFrequency() -> Math::Vector3* {
            if (!this->m_frequency) {
                auto frequency = this->m_vtkObject->GetFrequency();
                this->setFrequency(new Math::Vector3(frequency[0], frequency[1], frequency[2]));
            }

            return this->m_frequency;
        }

        auto PerlinNoise::setPhase(Math::Vector3* vector) -> void {
            if (this->m_phase) {
                this->m_phase->removeCallback(std::move(this->m_phaseCb));
            }

            this->m_phase = vector;

            if (vector) {
                vector->addCallback(std::move(this->m_phaseCb));
                this->updatePhase(std::move(*vector));
            }

            emit this->phaseChanged();
        }

        auto PerlinNoise::getPhase() -> Math::Vector3* {
            if (!this->m_phase) {
                auto phase = this->m_vtkObject->GetPhase();
                this->setPhase(new Math::Vector3(phase[0], phase[1], phase[2]));
            }

            return this->m_phase;
        }

        PerlinNoise::~PerlinNoise() {
            if (this->m_frequency) {
                this->m_frequency->removeCallback(std::move(this->m_frequencyCb));
            }

            if (this->m_phase) {
                this->m_phase->removeCallback(std::move(this->m_phaseCb));
            }
        }
    }
}
