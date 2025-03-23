import re
import gzip
import htmlmin
import csscompressor

def minify_html(content):
    """Minifies HTML and embedded CSS within <style> tags."""
    
    # Minify inline <style> CSS
    def minify_css_in_style_tags(match):
        css_content = match.group(1)
        return f"<style>{csscompressor.compress(css_content)}</style>"

    # Apply CSS minification to <style> blocks
    content = re.sub(r"<style>(.*?)</style>", minify_css_in_style_tags, content, flags=re.DOTALL)
    
    # Minify the entire HTML
    return htmlmin.minify(content, remove_empty_space=True, remove_all_empty_space=True)

def gzip_compress(content):
    """Gzips the given content."""
    return gzip.compress(content.encode('utf-8'))

def convert_to_decimal(byte_data):
    """Converts bytes to a comma-separated decimal list."""
    return ",".join(str(b) for b in byte_data)

def process_file(file_path):
    """Processes input HTML files and returns minified, gzipped, and decimal-converted data."""
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    minified = minify_html(content)
    gzipped = gzip_compress(minified)
    decimal_data = convert_to_decimal(gzipped)

    return decimal_data

def update_cpp_file(destination_file):
    """Updates the .cpp file with generated data between the start and end markers."""
    start_marker = "//generated code start"
    end_marker = "//generated code end"

    with open(destination_file, "r", encoding="utf-8") as f:
        cpp_content = f.read()

    pattern = re.compile(f"{start_marker}.*?{end_marker}", re.DOTALL)
    if not pattern.search(cpp_content):
        raise ValueError("Start and End markers not found in the destination file.")

    generated_code = f"""{start_marker}
#if ESPASYNCHTTPUPDATESERVER_MODE == 0
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
        static const uint8_t serverIndex[] PROGMEM = {{ {process_file("../www/stylized.html")} }};
    #else
        static const uint8_t serverIndex[] PROGMEM = {{ {process_file("../www/minimal.html")} }};
    #endif
#elif ESPASYNCHTTPUPDATESERVER_MODE == 1
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
        static const uint8_t serverIndex[] PROGMEM = {{ {process_file("../www/stylized-flash.html")} }};
    #else
        static const uint8_t serverIndex[] PROGMEM = {{ {process_file("../www/minimal-flash.html")} }};
    #endif
#elif ESPASYNCHTTPUPDATESERVER_MODE == 2
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
        static const uint8_t serverIndex[] PROGMEM = {{ {process_file("../www/stylized-fs.html")} }};
    #else
        static const uint8_t serverIndex[] PROGMEM = {{ {process_file("../www/minimal-fs.html")} }};
    #endif
#endif
{end_marker}"""

    updated_content = pattern.sub(generated_code, cpp_content)

    with open(destination_file, "w", encoding="utf-8") as f:
        f.write(updated_content)

if __name__ == "__main__":
    destination_file = "../src/ESPAsyncHTTPUpdateServer.cpp"  # Change to your actual .cpp file

    update_cpp_file(destination_file)

    print(f"Updated {destination_file} with generated code.")
