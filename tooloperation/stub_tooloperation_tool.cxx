#include "stub_tooloperation_tool.hxx"
#include <lxsdk/lxtoolui.h>
#include <lxsdk/lxarray.h>

StubTool_Visitor::StubTool_Visitor()
{

}

StubTool_Visitor::~StubTool_Visitor()
{

}

void StubTool_Visitor::evaluate()
{
	LXtFVector pos;
	editPoint.Pos(pos);
	LXtPointID pId = editPoint.ID();
	float totalWeight = 0.f;
	if (falloff_packet.test())
	{
		float current_weight = static_cast<float> (falloff_packet.Evaluate(pos, pId, nullptr));
		pos[1] = pos[1] + (Y * current_weight);
		LXtVector posD;
		posD[0] = pos[0];
		posD[1] = pos[1];
		posD[2] = pos[2];

		editPoint.SetPos(posD);
	}
}

Stub_Tool::Stub_Tool()
{
	CLxUser_PacketService	 sPkt;
	sPkt.NewVectorType(LXsCATEGORY_TOOL, v_type);
	sPkt.AddPacket(v_type, LXsP_TOOL_FALLOFF, LXfVT_GET);
	sPkt.AddPacket(v_type, LXsP_TOOL_SUBJECT2, LXfVT_GET);
	OffsetFalloff = sPkt.GetOffset(LXsCATEGORY_TOOL, LXsP_TOOL_FALLOFF);
	OffsetSubject = sPkt.GetOffset(LXsCATEGORY_TOOL, LXsP_TOOL_SUBJECT2);

	dyna_Add("y", LXsTYPE_FLOAT);
	idxY = 0;
}

void Stub_Tool::tool_Reset() 
{
}

LXtObjectID Stub_Tool::tool_VectorType() 
{
	return v_type.m_loc;
}

const char * Stub_Tool::tool_Order() 
{
	return LXs_ORD_ACTR;
}

LXtID4 Stub_Tool::tool_Task() 
{
	return LXi_TASK_ACTR;
}

unsigned Stub_Tool::tmod_Flags() 
{
	return LXfTMOD_AUTOACTIVATE;
}

void Stub_Tool::tmod_Initialize(ILxUnknownID vts, ILxUnknownID adjust, unsigned flags) 
{
	CLxUser_VectorStack	 vec(vts);
}

LxResult Stub_Tool::tmod_TestType(LXtID4 type) 
{
	switch (type) {
	case LXi_VPSPACE_MODEL:
	case LXi_VPSPACE_WORLD:
	case LXi_VPSPACE_MODEL2D:
		return LXe_TRUE;
	}
	return LXe_FALSE;

}

LxResult Stub_Tool::tmod_Enable(ILxUnknownID msg) 
{
	return LXe_OK;
}

LxResult Stub_Tool::tmod_Drop(void)
{
	return LXe_OK;
}

LxResult Stub_Tool::tool_GetOp(void** ppvObj, unsigned flags)
{
	*ppvObj = nullptr;
	StubTool_ToolOperation* toolop = StubTool_ToolOperation::Spawn(ppvObj);

	if (!toolop)
	{
		return LXe_FAILED;
	}
	double y;
	attr_GetFlt(idxY, &y);
	
	toolop->y = static_cast<float>(y);
	
	// copy the flags and packets
	toolop->OffsetFalloff = OffsetFalloff;
	toolop->OffsetSubject = OffsetSubject;
	toolop->flags = flags;	

	return LXe_OK;
}

LXtTagInfoDesc Stub_Tool::descInfo[] =
{
	{LXsTOOL_USETOOLOP, "."},
	{nullptr}
};

LxResult StubTool_ToolOperation::eltgrp_GroupCount(unsigned int* count)
{
	//count[0] = 1;
	return LXe_OK;
}

LxResult StubTool_ToolOperation::eltgrp_GroupName(unsigned int index, const char** name)
{
	//name[0] = "addedVertices";

	return LXe_OK;
}

LxResult StubTool_ToolOperation::eltgrp_GroupUserName(unsigned int index, const char** username)
{
	//username[0] = "@edgeSubdivideMessage@addedVertices@";

	return LXe_OK;
}

LxResult StubTool_ToolOperation::eltgrp_TestPoint(unsigned int index, LXtPointID point)
{
	/*std::set<LXtPointID>::iterator it = AddedPoints.find(point);
	if (it != AddedPoints.end())
	{
		return LXe_TRUE;
	}*/
	return LXe_FALSE;
}

StubTool_ToolOperation::StubTool_ToolOperation()
{

}

LxResult StubTool_ToolOperation::top_Evaluate(ILxUnknownID vts)
{
	CLxUser_VectorStack vectorStack(vts);
	if (!vectorStack.test())
	{
		return LXe_FAILED;
	}

	CLxUser_Mesh		 baseMesh;
	CLxUser_Mesh		 editMesh;
	CLxUser_MeshService srv_mesh;
	StubTool_Visitor		 vis;
	CLxUser_SelectionService srv_sel;

	CLxUser_LayerService	 s_layer;
	CLxUser_LayerScan	 scan;

	unsigned select_mode;
	srv_mesh.ModeCompose("select", nullptr, &select_mode);

	CLxUser_Subject2Packet subjectPacket;

	if (vectorStack.ReadObject(OffsetSubject, subjectPacket))
	{
		CLxUser_LayerScan layerScan;
		if (subjectPacket.BeginScan(LXf_LAYERSCAN_EDIT_EDGES, layerScan))
		{
			unsigned int layerCount = layerScan.NumLayers();
			for (unsigned int i = 0; i < layerCount; i++)
			{
				CLxUser_Mesh meshLoc;
				layerScan.EditMeshByIndex(i, editMesh);
				layerScan.BaseMeshByIndex(i, baseMesh);
				vectorStack.ReadObject(OffsetFalloff, vis.falloff_packet);
				
				vis.basePoint.fromMesh(baseMesh);
				vis.editPoint.fromMesh(editMesh);
				vis.Y = y;
				
				vis.editPoint.Enumerate(select_mode, vis, nullptr);
				

				layerScan.SetMeshChange(i, LXf_MESHEDIT_GEOMETRY | LXf_MESHEDIT_MAP_CONTINUITY | LXf_MESHEDIT_MAP_UV | LXf_MESHEDIT_MAP_OTHER | LXf_MESHEDIT_MAP_MORPH);


				if (flags & LXiTOOLOP_TOOLPIPE)
				{
					// manual tool stuff.

				}
				if (flags & LXiTOOLOP_PROCEDURAL)
				{
					// toolop specific stuff.
				}
			}
			layerScan.Apply();
			vis.falloff_packet.clear();
		}
	}
	return LXe_OK;
}

LxResult StubTool_Package::pkg_Attach(void** ppvObj)
{
	_inst_spawn.Alloc(ppvObj);

	return ppvObj[0] ? LXe_OK : LXe_FAILED;
}

LXtTagInfoDesc StubTool_Package::descInfo[] =
{
	{ LXsPKG_SUPERTYPE,	LXsITYPE_TOOLOP },
	{ LXsPKG_TOOL_TASK,	LXs_TASK_ACTR },
	{ LXsPKG_TOOL_ORDER, LXs_ORDPKG_ACTR },
	{ LXsPKG_TOOL_CHANNEL,	LXsICHAN_TOOL_OBJ },
	{LXsPMODEL_SELECTIONTYPES, LXsSELOP_TYPE_VERTEX},
	{ nullptr }
};

LxResult StubTool_Package::cui_UIHints(const char* channelName, ILxUnknownID hints)
{
	return LXe_OK;
}

LxResult StubTool_Package::pkg_TestInterface(const LXtGUID* guid)
{
	return _inst_spawn.TestInterfaceRC(guid);
}

LxResult StubTool_Package::pkg_SetupChannels(ILxUnknownID addChan)
{

	CLxUser_AddChannel ac(addChan);

	ac.NewChannel("y", LXsTYPE_FLOAT);
	
	// add new channels here.
	// They must also be added in the Modifier element constructor, in the exact same order.




	// leave this as last
	if (LXx_OK(ac.NewChannel(LXsICHAN_TOOL_OBJ, LXsTYPE_OBJREF)))
	{
		ac.SetInternal();
	}

	return LXe_OK;

}

LxResult StubTool_Package::cui_Enabled(const char* channelName, ILxUnknownID msg, ILxUnknownID item, ILxUnknownID chanRead)
{
	
	return LXe_OK;
}

LxResult StubTool_Package::cui_DependencyCount(const char* channelName, unsigned* count)
{
	return LXe_OK;
}

LxResult StubTool_Package::cui_DependencyByIndexName(const char* channelName, unsigned index, const char** depItemTypeName, const char** depChannelName)
{
	return LXe_OUTOFBOUNDS;
}

StubTool_ModifierElement::StubTool_ModifierElement(CLxUser_Evaluation& eval, ILxUnknownID item_obj)
{
	CLxUser_Item		 item(item_obj);
	_output_index = -1;

	if (item.test())
	{
		idxY = eval.AddChan(item, "y", LXfECHAN_READ);


		// leave me at the end.
		_output_index = eval.AddChan(item, LXsICHAN_TOOL_OBJ, LXfECHAN_WRITE); 
	}
}

void StubTool_ModifierElement::Eval(CLxUser_Evaluation& eval, CLxUser_Attributes& attr)
{
	CLxUser_ValueReference	 val_ref;

	ILxUnknownID object = nullptr;
	Stub_Tool* tool = Stub_Tool::Spawn((void**)&object);
	if (_output_index < 0)
		return;

	if (attr.ObjectRW(_output_index, val_ref))
	{
		val_ref.SetObject(object);
	}

	// this copies the values to the tool. The tool then sends them to the toolop.

	double y = attr.Float(idxY);
	
	CLxUser_Attributes toolAttr(object);
	if (toolAttr)
	{
		toolAttr.Set(idxY, y);
	}
	lx::UnkRelease(object);
}

const char* StubTool_ModifierServer::ItemType()
{
	/*
	 *	Associate the modifier with all items of our type.
	 */


	return STUB_TOOL_MESHOP;
}

CLxItemModifierElement* StubTool_ModifierServer::Alloc(CLxUser_Evaluation& eval, ILxUnknownID item_obj)
{
	return new StubTool_ModifierElement(eval, item_obj);
}
