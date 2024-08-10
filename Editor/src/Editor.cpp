#include "Editor.h"



Ivory::Application* Ivory::create_application() {
	return new Editor();


}

void ExampleLayer::on_event(Ivory::Event& e){
	Ivory::EventDispatcher dispatcher(e);

	dispatcher.dispatch<Ivory::MouseScrollEvent>(std::bind(&ExampleLayer::on_scroll, this, std::placeholders::_1));
}
