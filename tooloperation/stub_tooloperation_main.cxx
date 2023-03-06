#include "stub_tooloperation_tool.hxx"

void initialize()
{
	
	StubTool_Package::initialize();
	StubTool_Instance::initialize();
	StubTool_ToolOperation::initialize();
	Stub_Tool::initialize();
	StubTool_ModifierServer::initialize();

	Stub_Tool::initialize_manual_tool();
}
