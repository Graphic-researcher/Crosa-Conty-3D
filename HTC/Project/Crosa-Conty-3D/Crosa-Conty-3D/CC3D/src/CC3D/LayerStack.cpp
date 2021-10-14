#include "cc3d_pch.h"
#include "LayerStack.h"

namespace CC3D {

	///Layer:Operate from List begin
	///OverLayer:Operate from List end


	LayerStack::LayerStack()
	{
	}

	/// When app shut down,destory layer stack

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	///Push the layer to right place (before overlayer)
	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	///Push to back of list
	///overlayer always be after layer
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	/// when pop layer,we won't lose it,
	///because we can push back the layer and the layer is just pop temporally
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

 
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

}