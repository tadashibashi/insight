//
//  ai_wavtime.cpp
//  ai_wavtime
//
//  Created by Aaron Ishibashi on 3/25/22.
//
#include "ai_wavtime.hpp"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>

#include <ctpl_stl.h>
#include <insight/mpeg/mpeg_file.hpp>
#include <insight/wav/wave_file.hpp>
#include <insight/platform.hpp>

#include <insight/wav/bext_chunk.hpp>
#include <insight/wav/cue_chunk.hpp>
#include <insight/wav/fmt_chunk.hpp>
#include <insight/timecode.hpp>

#include <options/options.hpp>

using namespace insight;
using namespace std;

atomic<double> total_time = 0;

vector<string> output;
string out_filepath;
vector<string> csv_data;
long double g_framerate = 30;
long double g_subframe_base = 80.0;
bool g_write_csv = true;

const string g_default_csv_name = "Timecode_Output.csv";


/// Takes the literal start time in seconds and rounds it by timecode frame.
/// In practice cues should always start on-frame, so this function
long double round_start_time(long double seconds, long double base_rate, long double frame_rate)
{
    const long double timecode_fps = (frame_rate / base_rate);
    seconds *= timecode_fps;
    seconds = round(seconds);
    return seconds / timecode_fps;
}



void display_wave_info(wave::wave_file *wav)
{
    auto *bext = wav->get_chunk<wave::bext_chunk>();
    auto *cues = wav->get_chunk<wave::cue_chunk>();
    auto *fmt  = wav->get_chunk<wave::fmt_chunk>();
    
    long double start_seconds = 0;
    
    cout << get_filename_from_path(wav->filepath()) << '\n';
    
    if (!bext)
    {
        cout << get_filename_from_path(wav->filepath()) << " has no broadcast wave data...\n";
    }
    else
    {
        // Print sample start info for testing...
        start_seconds =
            bext->time_ref() / (long double)fmt->sample_rate();
        start_seconds = round_start_time(start_seconds, round(g_framerate), g_framerate);
        //bext->log();

        timecode start_tc(start_seconds, g_framerate, (int)g_subframe_base);
        timecode end_tc(start_seconds + wav->seconds(), g_framerate, (int)g_subframe_base);
        timecode length_tc(wav->seconds(), g_framerate, (int)g_subframe_base);

        string start_tc_str = start_tc.to_string();
        string end_tc_str = end_tc.to_string();
        string length_tc_str = length_tc.to_string();

        cout << "First Sample : " << bext->time_ref() << '\n';
        cout << "Sample rate  : " << fmt->sample_rate() << '\n';
        cout << "Start Seconds: " << start_seconds << '\n';
        
        cout << "Starts = " << start_tc_str << '\n';
        cout << "Ends   = " << end_tc_str << '\n';

        
        
        // CSV Format: Filename,Start Time,End,Length
        // Line-endings will be added at file writing.
        if (g_write_csv)
        {
            csv_data.emplace_back(wav->filename() + "," + start_tc_str + ","
                                  + end_tc_str + "," + length_tc_str);
        }
    }
    



    
    // Display cue names and positions
    if (cues)
    {
        // Print cue labels and timecode for testing...
        cout << "Markers\n";
        size_t longest_name = 0;
        size_t longest_num = 0;
        for (wave::cue &c : *cues)
        {
            if (c.name.length() > longest_name) longest_name = c.name.length();
            std::string id_str = to_string(c.id);
            if (id_str.length() > longest_num) longest_num = id_str.length();
        }
        
        for (wave::cue &c : *cues)
        {
            timecode cue_tc((long double)(c.position / (long double)fmt->sample_rate()) + start_seconds,
                            g_framerate, (int)g_subframe_base);
            printf("  [%*u] (%*s) = ", (int)longest_num, c.id, (int)longest_name, c.name.c_str());
            cout << cue_tc.to_string() << '\n';
        }
        
        cout << "---------------------------------------------\n\n";
    }
    else
    {
        cout << " No cue marker data to show\n";
    }
}

void parse_file(const string &path)
{
    string message;
    // Get the filename and extension from the full path
    string filename = get_filename_from_path(path);
    string ext      = get_file_ext_from_path(path);
   
    // Announce the current file to be scanned
    message += " " + filename + '\n';
   
    // Create the file reader based on file extension
    audio_file *file;
    if (ext == "mp3")
        file = new mpeg_file(path);
    else if (ext == "wav")
        file = new wave::wave_file(path);
    else
    {
        message += "File type not supported.\n";
        return;
    }

    // Get the time and add it to the total
    if (file->is_valid())
    {
       double seconds = file->seconds();
       
       message += " + " + to_string(seconds) + " seconds\n";
       total_time = total_time + seconds;
       
       if (dynamic_cast<wave::wave_file *>(file))
       {
           display_wave_info((wave::wave_file *)file);
       }
    }

    // Clean up
    delete file;
    output.emplace_back(message);
}

int
ai_wavtime::run(int *files_read, int *audio_files_counted)
{
    // Get user-selected file paths
    vector<string> user_paths;
    vector<string> collected_paths; // will include all subdirectory files as well.

    try {
        get_selected_browser_paths(user_paths);
        
        for (string &path : user_paths)
        {
            collect_files(path, collected_paths);
        }
    }
    catch (const exception &e)
    {
        cerr << "There was a problem while attempting to accessing selected finder paths. "
                "Does this program have the correct OS read permissions to the selected paths?:\n\n"
                << e.what() << "\n\n";
        return -1;
    }

    if (user_paths.empty())
    {
        cout << "No paths selected by the user.\n";
        return 0;
    }

    // Modify the csv filepath if necessary
    if (g_write_csv)
    {
        if (out_filepath.empty()) // Set a default csv path if the user didn't pass one in.
        {
            set_default_path:
            if (!user_paths.empty())
            {
                auto path = filesystem::path(user_paths[0]);
                if (path.has_parent_path())
                {
                    out_filepath = path.parent_path().string() + "/" + g_default_csv_name;
                }
                else
                {
                    out_filepath = path.string() + "/" + g_default_csv_name;
                }
            }
            cout << "Csv output path set to: " << out_filepath << '\n';
        }
        else
        {
            auto path = filesystem::path(out_filepath);
            if (!filesystem::exists(path.parent_path()))
            {
                std::cerr << "Csv file output path \"" << path.string() << "\" does not exist\n"
                    << "Writing to user-selected filepath instead.\n";
                goto set_default_path;
            }
            else
            {
                out_filepath = path.string();
            }
        }
    }
    cout << "Frame rate     : " << g_framerate << " fps\n" <<
            "Sub-frame base : " << g_subframe_base << "\n";
    
    cout << "Reading " << collected_paths.size() << " file(s)..." << "\n\n";
    
    ctpl::thread_pool p(40);
    
    // For each path that was selected by user in the browser...
    for (const string &path : collected_paths)
        p.push([&path](int id) {parse_file(path);});
    p.stop(true);
    
    cout << "\n\n";
    cout << " ----------------------------------------------\n";
    cout << " Wave file lengths\n";
    for (const string &message : output)
        cout << message;

    // Print the results!
    cout << " ----------------------------------------------\n";
    cout << " = "
    << setfill('0') << setw(2) << (int)(total_time / 3600) << ":"
    << setfill('0') << setw(2) << (int)(total_time / 60) % 60 << ":";
    cout.setf(ios::fixed);
    cout << setw(2) << setfill('0') << setprecision(2) << fmodf(total_time, 60.f) << " total\n\n";
    
    *files_read = (int)collected_paths.size();
    *audio_files_counted = (int)output.size();
    
    if (g_write_csv)
    {
        if (!csv_data.empty())
        {
            const char *csv_filepath = out_filepath.c_str();
            ofstream file(csv_filepath);
            if (file.is_open())
            {
                file << "Name,Start,End,Length\n";
                for (const string &str : csv_data)
                    file << str << '\n';

                // append total length
                timecode total_tc = timecode(total_time, g_framerate, (int)g_subframe_base);
                file << "Total,,," << total_tc.to_string() << '\n';
                file.close();
                cout << "Csv file was successfully written at \"" << csv_filepath << "\"\n";
            }
            else
            {
                cerr << "Could not write " << csv_filepath << ". Does the filepath exist, and does this program have the correct OS write permissions?\n";
            }
        }
        else
        {
            cout << "Csv file output was prevented since there was no audio data to write.\n";
        }
    }
    
    return EXIT_SUCCESS;
}

#include <chrono>

// Entry-point
int main(int argc, char * argv[])
{
    options opts(argc, argv);
    //opts.log();
    {
        // CSV file output option 'o'
        option out_opt;
        if (opts.get_option('o', &out_opt))
        {   // option exists
            g_write_csv = true;
            if (out_opt.has_arg())
                out_filepath = out_opt.arg();
        }
        else // option doesn't exist
        {
            g_write_csv = false;
        }

        // Subframe base 's'
        int subframe_base = 80;
        opts.get_arg('s', &subframe_base);
        if (subframe_base != 80 && subframe_base != 100)
        {
            cout << "Warning: unconventional subframe base of " << subframe_base << " was set.\n"
                << "Typically this value will be 80 or 100\n";
        }
        g_subframe_base = subframe_base;

        // Framerate 'r' TODO: allow for float values
        int framerate = 30;
        opts.get_arg('r', &framerate);

        g_framerate = framerate;
    }

    
    auto start = std::chrono::high_resolution_clock::now();
    int files_read = 0, audio_files_counted = 0;
    int result = ai_wavtime::run(&files_read, &audio_files_counted);
    auto stop = std::chrono::high_resolution_clock::now();
    
    cout << files_read << " file(s) read in " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.f << " seconds\n" << audio_files_counted << " of them were compatible audio files.\n";
    
    return result;
}
