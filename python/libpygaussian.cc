#include <boost/python.hpp>

using namespace boost::python;

void export_collections();
void export_tensor();
void export_core();
void export_mints();

BOOST_PYTHON_MODULE(libpygaussian)
{
    export_collections();        
    export_tensor();        
    export_core();        
    export_mints();        
}
