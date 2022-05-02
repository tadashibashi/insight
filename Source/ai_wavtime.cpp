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
#include <insight/wav/list_chunk.hpp>

#include <insight/options.hpp>


using namespace std;

atomic<double> total_time = 0;

vector<string> output;
vector<string> csv_data;
long double g_framerate = 30;
const long double subframe_base = 80.0;
bool g_write_csv = true;

// temp helper, move elsewhere later
void log_seconds_as_timecode(long double seconds, long double base_rate, long double frame_rate = 30.00)
{
    seconds *= (frame_rate / base_rate);
    long double frames = fmod(seconds, 1.0) / (1.0 / base_rate);
    
    //long double subframes = round(fmod(frames,1.0) * 80.0);
    cout << setfill('0') << setw(2) << round(seconds / 3600.0) << ":" // hours
         << setfill('0') << setw(2) << (int)round(seconds / 60.0) % 60 << ":" // minutes
         << setfill('0') << setw(2) << ((int)round(seconds) % 60) << ":" // seconds
    << setfill('0') << setw(2) << (int)round(frames) % (int)base_rate << '\n';
}




long double round_base(long double n, int base)
{
    return round(n * base) / (long double)base;
}

/// Rounds total timecode seconds by desired subframe base value.
/// @param tc_seconds        The time in Timecode seconds
/// @param base_rate          The number of frames in one timecode second (NOTE: this can differ from the actual frame rate.
///                      e.g. 29.97fps has a base_rate of 30 frames per timecode second.)
/// @param subframe_base The number of subframes to divide a frame with. Usually it is 80 (Logic Pro) or 100.
long double round_timecode_subframe(long double tc_seconds, long double base_rate, long double subframe_base)
{
    const long double secs_per_frame = 1.0/base_rate;
    return round(tc_seconds / secs_per_frame * subframe_base) / subframe_base * secs_per_frame;
}

/// Takes the literal start time in seconds and rounds it by timecode frame.
/// In practice cues should always start on-frame, so this function
long double round_start_time(long double seconds, long double base_rate, long double frame_rate)
{
    const long double timecode_fps = (frame_rate / base_rate);
    seconds *= timecode_fps;
    seconds = round(seconds);
    return seconds / timecode_fps;
}

string seconds_to_timecode_str(long double seconds, long double base_rate, long double frame_rate)
{
    std::cout << setprecision(4);
    // Convert seconds -> frames -> timecode seconds
    long double total_frames = seconds * frame_rate;
    //std::cout << "total frames: " << total_frames << '\n';
    long double tc_seconds = round_timecode_subframe(total_frames / base_rate, base_rate, subframe_base);
    

    //std::cout << "timecode seconds: " << tc_seconds << '\n';
    long double frames = fmod(tc_seconds, 1.0) / (1.0 / base_rate);
    //std::cout << "timecode frames : " << frames << '\n';
    
    long double subframes = round(fmod(frames, 1.0) * subframe_base * subframe_base) / subframe_base;
    //std::cout << "subframes nonrounded: " << fmod(frames, 1.0) * subframe_base << '\n';
    
    stringstream stream;
    stream << setfill('0') << setw(2) << (int)(tc_seconds / 3600.0) << ":" // hours
         << setfill('0') << setw(2) << (int)(tc_seconds / 60.0) % 60 << ":" // minutes
         << setfill('0') << setw(2) << ((int)tc_seconds % 60) << ":" // seconds
    << setfill('0') << setw(2) << (int)frames % (int)base_rate << '.'
    << setfill('0') << setw(2) << (int)subframes % (int)subframe_base;
    string str = stream.str();
    
    return stream.str();
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
        
        string start_timecode = seconds_to_timecode_str(start_seconds, round(g_framerate), g_framerate);
        string end_timecode = seconds_to_timecode_str(start_seconds + wav->seconds(), round(g_framerate), g_framerate);
        string length_timecode = seconds_to_timecode_str(wav->seconds(), round(g_framerate), g_framerate);
        
        cout << "First Sample : " << bext->time_ref() << '\n';
        cout << "Sample rate  : " << fmt->sample_rate() << '\n';
        cout << "Start Seconds: " << start_seconds << '\n';
        
        cout << "Starts = " << start_timecode << '\n';
        cout << "Ends   = " << end_timecode << '\n';

        
        
        //CSV Format: Filename,Start Time,End,Length
        // Line-endings will be added at file writing.
        if (g_write_csv)
        {
            csv_data.emplace_back(wav->filename() + "," + start_timecode + ","
                                  + end_timecode + "," + length_timecode);
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
            printf("  [%*u] (%*s) = ", (int)longest_num, c.id, (int)longest_name, c.name.c_str());
            cout << seconds_to_timecode_str((long double)(c.position / (long double)fmt->sample_rate()) + start_seconds, round(g_framerate), g_framerate) << '\n';
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
        cerr << "There was a problem while attempting to accessing selected finder paths:\n\n" << e.what() << "\n\n";
        return -1;
    }

    
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
        const char *csv_filepath = "/Users/aaron/Desktop/Timecode Output.csv";
        ofstream file(csv_filepath);
        if (file.is_open())
        {
            file << "Filename,Start Time,End,Length\n";
            for (const string &str : csv_data)
                file << str << '\n';
            
            // append total length
            string total_timecode_str = seconds_to_timecode_str(total_time, round(g_framerate), g_framerate);
            file << "Total Length,,," << total_timecode_str << '\n';
            file.close();
        }
        else
        {
            cerr << "Could not write " << csv_filepath << ". Does this program have the correct OS write permissions?\n";
        }
    }
    
    return EXIT_SUCCESS;
}

#include <chrono>

// Entry-point
int main(int argc, const char * argv[])
{
    options opts(argc, argv);
    opts.log();
    
    auto start = std::chrono::high_resolution_clock::now();
    int files_read = 0, audio_files_counted = 0;
    int result = ai_wavtime::run(&files_read, &audio_files_counted);
    auto stop = std::chrono::high_resolution_clock::now();
    
    cout << files_read << " files read in " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.f << " seconds\n" << audio_files_counted << " of them were compatible audio files.\n";
    
    return result;
}
