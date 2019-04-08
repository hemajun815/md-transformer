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
        this->_add_html_header();
        this->_process_content();
        this->_add_html_footer();
        return "Finished.";
    }

#pragma region : Funtional
  private:
    void _add_html_header()
    {
        this->m_file_out << "<!DOCTYPE html>" << std::endl;
        this->m_file_out << "<html lang=\"en\">" << std::endl;
        this->m_file_out << "<head>" << std::endl;
        this->m_file_out << "<meta charset=\"UTF-8\">" << std::endl;
        this->m_file_out << "<meta name=\"viewport\" content =\"width=device-width, initial-scale=1.0\">" << std::endl;
        this->m_file_out << "<meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">" << std::endl;
        this->m_file_out << "<title>Markdown by md-transformer</title>" << std::endl;
        this->m_file_out << "<link type=\"text/css\" rel=\"stylesheet\" href=\"./github-markdown.css\" />" << std::endl;
        this->m_file_out << "</head>" << std::endl;
        this->m_file_out << "<body>" << std::endl;
        this->m_file_out << "<div class=\"markdown-body\">" << std::endl;
    }
    void _process_content()
    {
        std::string line = "";
        while (!this->m_file_in.eof())
        {
            std::getline(this->m_file_in, line);
            if (line.length() > 0)
            {
                this->m_file_out << line << std::endl;
            }
        }
    }
    void _add_html_footer()
    {
        this->m_file_out << "</div>" << std::endl;
        this->m_file_out << "</body>" << std::endl;
        this->m_file_out << "</html>" << std::endl;
    }
#pragma endregion
};
}; // namespace hmj