/*******************************************************************************
 * Creator: Hemajun
 * Description: Transform md to html.
*******************************************************************************/
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

namespace hmj
{
class MDTransformer
{
  private:
    std::ifstream m_file_in;
    std::ofstream m_file_out;

    bool m_in_code_block;
    bool m_in_blockquote;
    bool m_in_unordered_list;

  public:
    MDTransformer(const std::string &file_in, const std::string &file_out)
    {
        this->m_file_in.open(file_in, std::ios::in);
        this->m_file_out.open(file_out, std::ios::out | std::ios::trunc);
        this->m_in_code_block = false;
        this->m_in_blockquote = false;
        this->m_in_unordered_list = false;
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
            this->_process_header(line);
            this->_process_img_link(line);
            this->_process_a_link(line);
            this->_process_code(line);
            this->_process_inline_code(line);
            this->_process_bold(line);
            this->_process_italic(line);
            this->_process_hr(line);
            this->_process_unordered_list(line);
            this->_process_blockquote(line);
            this->_process_paragraph(line);
            if (line.length() > 0)
                this->m_file_out << line << std::endl;
        }
    }
    void _process_header(std::string &str)
    {
        auto count = 0;
        for (auto ch : str)
        {
            if (ch != '#')
                break;
            count++;
        }
        if (count != 0 && str.data()[count] == ' ')
        {
            std::stringstream ss;
            ss << "<h" << count << ">"
               << str.substr(count + 1)
               << "</h" << count << ">";
            str.clear();
            ss >> str;
        }
    }
    void _process_img_link(std::string &str)
    {
        std::regex reg("!\\[.+\\]\\(.+\\)");
        std::sregex_iterator it(str.begin(), str.end(), reg);
        for (auto it = std::sregex_iterator(str.begin(), str.end(), reg); it != std::sregex_iterator(); it++)
        {
            auto img_href = this->_substr(it->str(), '(', ')');
            auto img_text = this->_substr(it->str(), '[', ']');
            auto img_tag = "<img alt=\"" + img_text + "\" src=\"" + img_href + "\" />";
            str.replace(str.find(it->str()), it->str().length(), img_tag);
        }
    }
    void _process_a_link(std::string &str)
    {
        std::regex reg("\\[.+\\]\\(.+\\)");
        std::sregex_iterator it(str.begin(), str.end(), reg);
        for (auto it = std::sregex_iterator(str.begin(), str.end(), reg); it != std::sregex_iterator(); it++)
        {
            auto a_href = this->_substr(it->str(), '(', ')');
            auto a_text = this->_substr(it->str(), '[', ']');
            auto a_tag = "<a href=\"" + a_href + "\">" + a_text + "</a>";
            str.replace(str.find(it->str()), it->str().length(), a_tag);
        }
    }
    void _process_code(std::string &str)
    {
        if (str.compare("```") == 0)
        {
            str = this->m_in_code_block ? "</code></pre>" : "<pre><code>";
            this->m_in_code_block = !this->m_in_code_block;
        }
    }
    void _process_inline_code(std::string &str)
    {
        this->_process_with_regex(str, "`", "code");
    }
    void _process_bold(std::string &str)
    {
        this->_process_with_regex(str, "\\*{2}", "strong", 2);
    }
    void _process_italic(std::string &str)
    {
        this->_process_with_regex(str, "\\*", "em", 1);
    }
    void _process_hr(std::string &str)
    {
        if (str.compare("---") == 0)
        {
            str = "<hr></hr>";
        }
    }
    void _process_unordered_list(std::string &str)
    {
        if (str.substr(0, 2) == "- ")
        {
            str = "<li>" + str.substr(2) + "</li>";
            if (!this->m_in_unordered_list)
            {
                str = "<ul>" + str;
                this->m_in_unordered_list = !this->m_in_unordered_list;
            }
        }
        else if (str.length() == 0 && this->m_in_unordered_list)
        {
            str = "</ul>";
            this->m_in_unordered_list = !this->m_in_unordered_list;
        }
    }
    void _process_blockquote(std::string &str)
    {
        if (str.substr(0, 2) == "> ")
        {
            str = "<p>" + str.substr(2) + "</p>";
            if (!this->m_in_blockquote)
            {
                str = "<blockquote>" + str;
                this->m_in_blockquote = !this->m_in_blockquote;
            }
        }
        else if (str.length() == 0 && this->m_in_blockquote)
        {
            str = "</blockquote>";
            this->m_in_blockquote = !this->m_in_blockquote;
        }
    }
    void _process_paragraph(std::string &str)
    {
        if (str.length() > 0 && !this->m_in_code_block && *str.begin() != '<' && *str.end() != '>')
            str = "<p>" + str + "</p>";
    }
    void _add_html_footer()
    {
        this->m_file_out << "</div>" << std::endl;
        this->m_file_out << "</body>" << std::endl;
        this->m_file_out << "</html>" << std::endl;
    }
#pragma endregion
#pragma region : Utilities
  private:
    std::string _substr(const std::string &str, const char &start, const char &end)
    {
        auto pos_s = str.find_first_of(start);
        auto pos_e = str.find_last_of(end);
        auto len = pos_e - pos_s;
        return str.substr(pos_s + 1, len - 1);
    }
    void _process_with_regex(std::string &str, const std::string &md_tag, const std::string &html_tag, int len_tag = -1)
    {
        if (len_tag == -1)
            len_tag = md_tag.length();
        std::regex reg(md_tag + ".+" + md_tag);
        std::sregex_iterator it(str.begin(), str.end(), reg);
        for (auto it = std::sregex_iterator(str.begin(), str.end(), reg); it != std::sregex_iterator(); it++)
        {
            auto text = it->str().substr(len_tag, it->str().length() - len_tag * 2);
            auto html = "<" + html_tag + ">" + text + "</" + html_tag + ">";
            str.replace(str.find(it->str()), it->str().length(), html);
        }
    }
#pragma endregion
};
}; // namespace hmj