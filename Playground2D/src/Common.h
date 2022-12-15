#include "Logger.h"

#define PI 3.145f

#define SHIFT_BIT(x) 1 << x

#ifdef PLAYGROUND_2D_DEBUG
#define PG_CORE_TRACE(...)	Logger::GetCoreLogger()->trace(__VA_ARGS__);
#define PG_CORE_WARN(...)	Logger::GetCoreLogger()->warn(__VA_ARGS__);
#define PG_CORE_ERROR(...)	Logger::GetCoreLogger()->error(__VA_ARGS__);
#define PG_CORE_INFO(...)	Logger::GetCoreLogger()->info(__VA_ARGS__);
#define PG_CORE_ASSERT(x, ...) if(x) Logger::GetCoreLogger()->info(__VA_ARGS__); else Logger::GetCoreLogger()->error(__VA_ARGS__)

#define PG_TRACE(...)	Logger::GetClientLogger()->trace(__VA_ARGS__);
#define PG_WARN(...)	Logger::GetClientLogger()->warn(__VA_ARGS__);
#define PG_ERROR(...)	Logger::GetClientLogger()->error(__VA_ARGS__);
#define PG_INFO(...)	Logger::GetClientLogger()->info(__VA_ARGS__);
#define PG_ASSERT(x, ...) if(x) Logger::GetClientLogger()->info(__VA_ARGS__); else Logger::GetClientLogger()->error(__VA_ARGS__)
#else
#define PG_CORE_TRACE(...)	
#define PG_CORE_WARN(...)	
#define PG_CORE_ERROR(...)	
#define PG_CORE_INFO(...)
#define PG_CORE_ASSERT(x, ...)

#define PG_TRACE(...)	
#define PG_WARN(...)	
#define PG_ERROR(...)	
#define PG_INFO(...)
#define PG_ASSERT(x, ...)
#endif

