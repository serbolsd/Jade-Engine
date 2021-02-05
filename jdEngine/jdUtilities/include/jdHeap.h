#pragma once
#include "jdPlatformTypes.h"
#include "jdStdHeaders.h"

namespace jdEngineSDK {
		struct Allocation	{
				Allocation* back = nullptr;
#if DEBUG
				INFOALLOC
						File // de donde (el archivo)
						Size // de cuanto
#endif
				void* pointerPos = nullptr;
				SIZE_T size = 0;
				Allocation* next = nullptr;
		};
		class CHeap
		{
		 public:
				CHeap(SIZE_T size) : m_memSize(size){
						//m_pData = alloc(size);
						m_pData = new UCHAR[size];
				};
				~CHeap();

	 	public:
				void* alloc(SIZE_T size) {
						Allocation newAlloc;
						
						UCHAR* plastData = m_pData;
						//newAlloc.size = size;
						
						for (auto& myAlloc : m_allocs)	{
								//if (plastData)
								if (nullptr == myAlloc.next)
								{
										int* a;
										a + 1;
										//plastData = myAlloc.pointerPos + 2;
										break;
								}
							//	SIZE_T freeSize = myAlloc.next - (myAlloc.size + sizeof(Allocation));
						}
				}
				//realloc or mem_resize
				void* realloc(SIZE_T size) {

				}
	 	public:
				SIZE_T m_memSize;
				UCHAR* m_pData;
				List<Allocation> m_allocs;
		};
}