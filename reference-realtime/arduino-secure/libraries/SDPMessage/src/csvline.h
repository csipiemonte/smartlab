/**
 * \file csvline.h
 * \brief Manage a line of a csv file.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef CSVLINE_H_
#define CSVLINE_H_

namespace sdp
{
  namespace message
  {

    /**
     * The CSVLine class describes a generic line of a CSV file.
     *
     * \note It is has been fixed a max size for the line size (128 chars).
     *
     */
    class CSVLine
    {
      public:

        //! Max size for a CSV line
        static size_t LINE_SIZE;

        /**
         * Default Constructor.
         *
         */
        CSVLine();

        /**
         * Copy-Constructor
         *
         */
        CSVLine(const CSVLine& l);

        /**
         * Destructor.
         *
         */
        virtual ~CSVLine();

        void set(const char* line, size_t length);

      private:
        //! Internal buffer for storage the CSV line
        char *m_line;

        //! line length
        size_t m_length;

        //! Pointer to the array, its elements point to the first character og each field
        size_t *m_fields;

        //! Number of fields
        byte m_nf;

        //! Flag  to check if it is a copy
        bool m_isCopy;
    };

  } /* namespace message */
} /* namespace sdp */

#endif /* CSVLINE_H_ */
