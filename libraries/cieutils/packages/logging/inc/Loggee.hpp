#ifndef CIE_CIEUTILS_LOGGEE_HPP
#define CIE_CIEUTILS_LOGGEE_HPP

// --- Internal Includes ---
#include "cieutils/packages/logging/inc/Logger.hpp"
#include "cieutils/packages/logging/inc/LogBlock.hpp"
#include "cieutils/packages/types/inc/NamedObject.hpp"

#include "cieutils/packages/concepts/inc/streamable.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <string>


// Logger message flags
namespace cie {
const size_t LOG_TYPE_MESSAGE   = 0;
const size_t LOG_TYPE_WARNING   = 1;
const size_t LOG_TYPE_ERROR     = 2;
}


namespace cie::utils {


class Loggee : public NamedObject
{
public:
    Loggee( Logger& r_logger, 
            const std::string& r_instanceName );
    ~Loggee();

    void log(   const std::string& message,
                size_t messageType = LOG_TYPE_MESSAGE );
    void logID( const std::string& message,
                size_t id,
                size_t messageType = LOG_TYPE_MESSAGE );

    [[nodiscard]] size_t tic();
    void toc(   size_t timerID,
                bool reset = true );
    void toc(   const std::string& message,
                size_t timerID,
                bool reset = true );

    LogBlock newBlock( const std::string& r_name ) { return _logger->newBlock(r_name); }

    void separate();

    FileManager& fileManager();

protected:
    Logger& logger();

private:
    Logger*         _logger;
    size_t          _timerID;
};


Loggee& operator<<( Loggee& r_loggee,
                    const std::string& r_message );


template <class ContainerType>
requires concepts::STLContainer<ContainerType>
         && concepts::StringStreamable<typename ContainerType::value_type>
Loggee& operator<<( Loggee& r_loggee,
                    const ContainerType& r_messageContainer );


template <concepts::StringStreamable MessageType>
Loggee& operator<<( Loggee& r_loggee,
                    const MessageType& r_message );


} // namespace cie::utils

#include "cieutils/packages/logging/impl/Loggee_impl.hpp"

#endif