#pragma once

namespace vul::Graphics {
	class Window {
	public:
		Window();
		~Window();

		inline void* GetRaw() const { return m_Raw; }

		virtual bool ShouldClose() const = 0;

	protected:
		void* m_Raw = nullptr;
	};
}