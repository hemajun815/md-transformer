/*******************************************************************************
 * Creator: Hemajun
 * Description: Transform md to html.
*******************************************************************************/
#include <fstream>
#include <string>

namespace hmj
{
class MDTransformer
{
  private:
    std::ifstream m_file_in;
    std::ofstream m_file_out;

  public:
    MDTransformer(const std::string &file_in, const std::string &file_out)
    {
        this->m_file_in.open(file_in, std::ios::in);
        this->m_file_out.open(file_out, std::ios::out | std::ios::trunc);
    }
    ~MDTransformer()
    {
        if (this->m_file_in.is_open())
            this->m_file_in.close();
        if (this->m_file_out.is_open())
            this->m_file_out.close();
    }
    std::string transform()
    {
        if (!this->m_file_in.good() || !this->m_file_out.good())
            return "File error.";
        return "Finished.";
    }
};
}; // namespace hmj