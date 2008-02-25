#ifndef OUT_H_
#define OUT_H_

#include <string>

#include "zypp/base/NonCopyable.h"
#include "zypp/base/Exception.h"
#include "zypp/Url.h"

#include "prompt.h"

/**
 * 
 * - Logger (DBG, MIL, ...) must be in place
 * - out.XX wherever output is needed
 * 
 */
class Out : private zypp::base::NonCopyable
{
public:
  /** Verbosity levels. */
  typedef enum
  {
    /** Only important messages (no progress or status, only the result). */
    QUIET  = 0,
    /**  */
    NORMAL = 1,
    /** More detailed description of the operations. */
    HIGH   = 2,
    DEBUG  = 3
  } Verbosity;

  typedef enum
  {
    TYPE_NORMAL = 1,
    TYPE_XML    = 2,
    TYPE_ALL    = 0xff
  } Type;

public:
  Out(Verbosity verbosity = NORMAL) : _verbosity(verbosity) {}
  virtual ~Out();

public:
  /**
   * Show an info message.
   * 
   * \param msg       The message to be displayed.
   * \param verbosity Minimal level o verbosity in which the message will be
   *                  shown. Out::QUIET means the message will be always be
   *                  displayed. Out::HIGH means the message will be displayed
   *                  only if the current verbosity level is HIGH (-v) or DEBUG
   *                  (-vv).
   * \param mask      Determines the types of output for which is this message
   *                  intended. By default, the message will be shown in all
   *                  types of output. 
   */
  virtual void info(const std::string & msg, Verbosity verbosity = NORMAL, Type mask = TYPE_ALL) = 0;

  /**
   * Show a warning.
   * 
   * \param msg       The warning message to be displayed.
   * \param verbosity Minimal level o verbosity in which the message will be
   *                  shown. Out::QUIET means the message will be always be
   *                  displayed. Out::HIGH means the message will be displayed
   *                  only if the current verbosity level is HIGH (-v) or DEBUG
   *                  (-vv).
   * \param mask      Determines the types of output for which is this message
   *                  intended. By default, the message will be shown in all
   *                  types of output. 
   */
  virtual void warning(const std::string & msg, Verbosity verbosity = NORMAL, Type mask = TYPE_ALL) = 0;

  /**
   * Show an error message and an optional hint.
   * 
   * An error message should be shown regardless of the verbosity level.
   * 
   * \param problem_desc Problem description (what happend)
   * \param hint         Hint for the user (what to do, or explanation)
   */
  virtual void error(const std::string & problem_desc, const std::string & hint = "") = 0;
  
  /**
   * Prints the problem description caused by an exception, its cause and,
   * optionaly, a hint for the user.
   * 
   * \param e Exception which caused the problem.
   * \param Problem description for the user.
   * \param Hint for the user how to cope with the problem.
   */
  virtual void error(const zypp::Exception & e,
                     const std::string & problem_desc,
                     const std::string & hint = "") = 0;

  // virtual void table()
  // virtual void text()

  /** \name Progress */
  //@{
  virtual void progressStart(const std::string & id,
                             const std::string & label,
                             bool is_tick = false) = 0;
  virtual void progress(const std::string & id,
                        const std::string & label,
                        int value = -1) = 0;
  virtual void progressEnd(const std::string & id,
                           const std::string & label,
                           bool error = false) = 0; // might be a string with error message instead
  //@}

  /** \name Download progress with download rate */
  //@{
  /**
   * \param uri   Uri of the file to download. 
   */
  virtual void dwnldProgressStart(const zypp::Url & uri) = 0;

  /**
   * \param uri   Uri of the file being downloaded. 
   * \param value Value of the progress in percents. -1 if unknown.
   * \param rate  Download rate. -1 if unknown.
   */
  virtual void dwnldProgress(const zypp::Url & uri,
                             int value = -1,
                             int rate = -1) = 0;
  /**
   * \param uri   Uri of the file to download. 
   * \param rate  Final download rate. -1 if unknown.
   * \param error Error flag - did the download finish with error?
   */
  virtual void dwnldProgressEnd(const zypp::Url & uri,
                                int rate = -1,
                                bool error = false) = 0;
  //@}

  virtual void prompt(PromptId id,
                      const std::string & prompt,
                      const std::string & answer_hint) = 0;
public:
  /** Get current verbosity. */
  Verbosity verbosity() { return _verbosity; }

  /** Set current verbosity. */
  void setVerbosity(Verbosity verbosity) { _verbosity = verbosity; }

  /** Return the type of the instance. */
  Type type() { return _type; }

protected:
  /**
   * Determine whether the output is intended for the particular type.
   */
  virtual bool mine(Type type) = 0;

  /**
   * Determine whether to show progress.
   * 
   * \return <tt>true</tt> if the progress should be filtered out,
   *         <tt>false</tt> if it should be shown.
   */
  virtual bool progressFilter();

  /**
   * Return a zypp::Exception as a string suitable for output.
   */
  virtual std::string zyppExceptionReport(const zypp::Exception & e);

private:
  Verbosity _verbosity;
  Type      _type;
};

#endif /*OUT_H_*/