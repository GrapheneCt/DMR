#include <kernel.h>
#include <taihen.h>

#define BUDGET_ID_CURRENT 0

static tai_hook_ref_t s_hookRef[2];

// This is needed due to the way BudgetIDtoBudgetObj() function in SceProcessmgr is designed
int sceKernelGrowBudget_patched(SceUID budgetId, int index, SceSize size)
{
	int ret = TAI_NEXT(sceKernelGrowBudget_patched, s_hookRef[0], budgetId, index, size);
	if (ret == SCE_KERNEL_ERROR_DIFFERENT_UID_CLASS)
	{
		ret = TAI_NEXT(sceKernelGrowBudget_patched, s_hookRef[0], BUDGET_ID_CURRENT, index, size);
	}

	return ret;
}

// Can safely always return 1, SceAppMgr only uses it only for budget management
int sceSblACMgrIsWebCoreOrWebKitProcess_patched(SceUID pid)
{
	TAI_NEXT(sceSblACMgrIsWebCoreOrWebKitProcess_patched, s_hookRef[1], pid);
	return 1;
}

//MAIN
int __module_start(SceSize args, const void * argp)
{
	tai_module_info_t info;
	info.size = sizeof(tai_module_info_t);
	taiGetModuleInfoForKernel(KERNEL_PID, "SceAppMgr", &info);

	// Allow apps with any budget type (by default only mini budget apps are allowed)
	switch (info.module_nid)
	{
	case 0x94CEFE4B: // 3.55
	case 0xDFBC288C: // 3.57
	case 0xDBB29DB7: // 3.60
		taiHookFunctionImportForKernel(KERNEL_PID, &s_hookRef[0], "SceAppMgr", 0x7A69DE86, 0x029378AC, sceKernelGrowBudget_patched);
	case 0x1C9879D6: // 3.65
		taiHookFunctionImportForKernel(KERNEL_PID, &s_hookRef[0], "SceAppMgr", 0xEB1F8EF7, 0x48DD3348, sceKernelGrowBudget_patched);
		break;
	default:
		return SCE_KERNEL_START_NO_RESIDENT;
	}

	taiInjectDataForKernel(KERNEL_PID, info.modid, 0, 0xF774, "\xbb\xb9", 2);
	taiHookFunctionImportForKernel(KERNEL_PID, &s_hookRef[1], "SceAppMgr", 0x9AD8E213, 0x0139FC20, sceSblACMgrIsWebCoreOrWebKitProcess_patched);

	return SCE_KERNEL_START_SUCCESS;
}

int __module_stop(SceSize args, const void * argp)
{
	return SCE_KERNEL_STOP_SUCCESS;
}