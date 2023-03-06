#pragma once

#include <lxsdk/lx_tool.hpp>
#include <lxsdk/lx_toolui.hpp>
#include <lxsdk/lx_vmodel.hpp>
#include <lxsdk/lx_vector.hpp>
#include <lxsdk/lxu_attributes.hpp>
#include <lxsdk/lx_plugin.hpp>
#include <lxsdk/lx_layer.hpp>
#include <lxsdk/lx_mesh.hpp>
#include <lxsdk/lx_log.hpp>
#include <lxsdk/lx_draw.hpp>
#include <lxsdk/lx_value.hpp>
#include <lxsdk/lx_handles.hpp>
#include <lxsdk/lx_message.hpp>
#include <lxsdk/lxu_math.hpp>
#include <lxsdk/lxvp.h>
#include <lxsdk/lx_select.hpp>
#include <lxsdk/lx_seltypes.hpp>
#include <lxsdk/lx_package.hpp>
#include <lxsdk/lxu_select.hpp>
#include <lxsdk/lxu_matrix.hpp>
#include <lxsdk/lxu_command.hpp>
#include <lxsdk/lxu_modifier.hpp>
#include <lxsdk/lxw_tool.hpp>
#include <lxsdk/lxw_vmodel.hpp>
#include <lxsdk/lxw_channelui.hpp>
#include <lxsdk/lxw_pmodel.hpp>

#define STUB_TOOL_MESHOP "pmodel.stub.tool"
#define STUB_TOOL_MANUAL "stub.tool"


class Stub_Tool :
	public CLxImpl_Tool,
	public CLxImpl_ToolModel,
	public CLxDynamicAttributes
{
public:
	Stub_Tool();

	void		tool_Reset() override;
	LXtObjectID	tool_VectorType() override;
	const char *	tool_Order() override;
	LXtID4		tool_Task() override;
	unsigned	tmod_Flags() override;
	void		tmod_Initialize(ILxUnknownID vts, ILxUnknownID adjust, unsigned flags) override;
	LxResult	tmod_TestType(LXtID4 type) override;
	LxResult	tmod_Enable(ILxUnknownID msg) override;

	CLxUser_VectorType	 v_type;

	static LXtTagInfoDesc descInfo[];
	virtual LxResult tmod_Drop(void) override;

	static void initialize()
	{
		CLxGenericPolymorph* srv;
		srv = new CLxPolymorph<Stub_Tool>;
		srv->AddInterface(new CLxIfc_Tool        <Stub_Tool>);
		srv->AddInterface(new CLxIfc_ToolModel   <Stub_Tool>);
		srv->AddInterface(new CLxIfc_Attributes <Stub_Tool>);
		srv->AddInterface(new CLxIfc_AttributesUI <Stub_Tool>);
		srv->AddInterface(new CLxIfc_StaticDesc<Stub_Tool>);
		lx::AddSpawner(STUB_TOOL_MESHOP".tool", srv);
	}

	static void initialize_manual_tool()
	{
		CLxGenericPolymorph* srv;
		srv = new CLxPolymorph<Stub_Tool>;
		srv->AddInterface(new CLxIfc_Tool        <Stub_Tool>);
		srv->AddInterface(new CLxIfc_ToolModel   <Stub_Tool>);
		srv->AddInterface(new CLxIfc_Attributes <Stub_Tool>);
		srv->AddInterface(new CLxIfc_AttributesUI <Stub_Tool>);
		srv->AddInterface(new CLxIfc_StaticDesc<Stub_Tool>);
		lx::AddServer(STUB_TOOL_MANUAL, srv);
	}

	static Stub_Tool* Spawn(void** ppvObj)
	{
		static CLxSpawner<Stub_Tool> spawner(STUB_TOOL_MESHOP".tool");
		return spawner.Alloc(ppvObj);
	}

	unsigned OffsetFalloff;
	unsigned OffsetSubject;
	LxResult tool_GetOp(void** ppvObj, unsigned flags) override;

private:
	int idxY;
};

class StubTool_Visitor : public CLxVisitor
{

public:
	StubTool_Visitor();
	~StubTool_Visitor();
	virtual void evaluate() override;

public:
	CLxUser_Point		 basePoint;
	CLxUser_Point		 editPoint;
	
	CLxUser_FalloffPacket falloff_packet;

	float Y;
	
};

class StubTool_Instance : public CLxImpl_PackageInstance
{
public:
	static void initialize()
	{
		CLxGenericPolymorph* srv = nullptr;
		srv = new CLxPolymorph						<StubTool_Instance>;
		srv->AddInterface(new CLxIfc_PackageInstance	<StubTool_Instance>);
		lx::AddSpawner(STUB_TOOL_MESHOP".inst", srv);
	}
};


class StubTool_Package : public CLxImpl_Package, public CLxImpl_ChannelUI
{
public:
	static void initialize()
	{
		CLxGenericPolymorph* srv = nullptr;
		srv = new CLxPolymorph						<StubTool_Package>;
		srv->AddInterface(new CLxIfc_Package		<StubTool_Package>);
		srv->AddInterface(new CLxIfc_ChannelUI		<StubTool_Package>);
		srv->AddInterface(new CLxIfc_StaticDesc		<StubTool_Package>);
		lx::AddServer(STUB_TOOL_MESHOP, srv);
	}

	StubTool_Package() : _inst_spawn(STUB_TOOL_MESHOP".inst") {}

	LxResult pkg_Attach(void** ppvObj)		override;

	static LXtTagInfoDesc	 descInfo[];


	LxResult cui_UIHints(const char* channelName, ILxUnknownID hints) override;
	LxResult pkg_TestInterface(const LXtGUID* guid) override;


	LxResult pkg_SetupChannels(ILxUnknownID addChan) override;


	LxResult cui_Enabled(const char* channelName, ILxUnknownID msg, ILxUnknownID item, ILxUnknownID chanRead) override;


	LxResult cui_DependencyCount(const char* channelName, unsigned* count) override;


	LxResult cui_DependencyByIndexName(const char* channelName, unsigned index, const char** depItemTypeName, const char** depChannelName) override;

private:
	CLxSpawner <StubTool_Instance>	 _inst_spawn;

};



class StubTool_ToolOperation : public CLxImpl_ToolOperation, public CLxImpl_MeshElementGroup
{
public:
	static void initialize()
	{
		CLxGenericPolymorph* srv = new CLxPolymorph<StubTool_ToolOperation>;
		srv->AddInterface(new CLxIfc_ToolOperation<StubTool_ToolOperation>);
		srv->AddInterface(new CLxIfc_MeshElementGroup<StubTool_ToolOperation>);
		lx::AddSpawner(STUB_TOOL_MESHOP".toolop", srv);
	}

	static StubTool_ToolOperation* Spawn(void** ppvObj)
	{
		static CLxSpawner<StubTool_ToolOperation> spawner(STUB_TOOL_MESHOP".toolop");
		return spawner.Alloc(ppvObj);
	}


	LxResult eltgrp_GroupCount(unsigned int* count) override;


	LxResult eltgrp_GroupName(unsigned int index, const char** name) override;


	LxResult eltgrp_GroupUserName(unsigned int index, const char** username) override;


	LxResult eltgrp_TestPoint(unsigned int index, LXtPointID point) override;

	std::set<LXtPointID> AddedPoints;

public:
	StubTool_ToolOperation();
	virtual LxResult top_Evaluate(ILxUnknownID vts) override;
	unsigned OffsetFalloff;
	unsigned OffsetSubject;

	float y;
	unsigned flags;

};


class StubTool_ModifierElement : public CLxItemModifierElement
{
public:
	StubTool_ModifierElement(CLxUser_Evaluation& eval, ILxUnknownID		 item_obj);

	void
		Eval(CLxUser_Evaluation& eval, CLxUser_Attributes& attr)			override;

private:
	int			 _output_index;
	unsigned idxY;
};

class StubTool_ModifierServer : public CLxItemModifierServer
{
public:
	static void initialize()
	{
		CLxGenericPolymorph* srv = new CLxPolymorph<StubTool_ModifierServer>;
		srv->AddInterface(new CLxIfc_EvalModifier<StubTool_ModifierServer>);
		srv->AddInterface(new CLxIfc_TagDescription<StubTool_ModifierServer>);
		lx::AddServer(STUB_TOOL_MESHOP".mod", srv);
	}

	const char* ItemType()						override;

	CLxItemModifierElement* Alloc(CLxUser_Evaluation& eval, ILxUnknownID		 item_obj)		override;
};
