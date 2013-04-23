using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Platform.Test;

namespace TextTests.StringBuilder
{
    class StringBuilderTests : IMFTestInterface
    {
        #region IMFTestInterface Members

        [TearDown]
        public void CleanUp()
        {
        }

        [SetUp]
        public InitializeResult Initialize()
        {
            return InitializeResult.ReadyToGo;
        }

        #endregion

        #region Fields

        System.Text.StringBuilder stringBuilder = new System.Text.StringBuilder();

        #endregion

        #region Append Tests
        
        [TestMethod]
        public MFTestResults Test_0_AppendTest_0()
        {

            bool result = false;

            stringBuilder.Append(true);

            result = stringBuilder.ToString() == Boolean.TrueString;

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Append(false);

            result = stringBuilder.ToString() == string.Concat(Boolean.TrueString, Boolean.FalseString);

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Append(byte.MinValue);

            result = stringBuilder.ToString() == string.Concat(Boolean.TrueString, Boolean.FalseString, byte.MinValue);

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Append(new char[] { 'x','a' });

            result = stringBuilder.ToString() == string.Concat(Boolean.TrueString, Boolean.FalseString, byte.MinValue, char.MinValue, "xa");

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Append(double.Epsilon);

            result = stringBuilder.ToString() == string.Concat(Boolean.TrueString, Boolean.FalseString, byte.MinValue, char.MinValue, "xa", double.Epsilon.ToString());

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Append(float.Epsilon);

            result = stringBuilder.ToString() == string.Concat(Boolean.TrueString, Boolean.FalseString, byte.MinValue, char.MinValue, "xa", double.Epsilon.ToString(), float.Epsilon.ToString());

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Append(int.MaxValue);

            result = stringBuilder.ToString() == string.Concat(Boolean.TrueString, Boolean.FalseString, byte.MinValue, char.MinValue, "xa", double.Epsilon.ToString(), float.Epsilon.ToString(), int.MaxValue);

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Append(long.MaxValue);

            result = stringBuilder.ToString() == string.Concat(Boolean.TrueString, Boolean.FalseString, byte.MinValue, char.MinValue, "xa", double.Epsilon.ToString(), float.Epsilon.ToString(), int.MaxValue, long.MaxValue);

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Append((object)"string");

            result = stringBuilder.ToString() == string.Concat(Boolean.TrueString, Boolean.FalseString, byte.MinValue, char.MinValue, "xa", double.Epsilon.ToString(), float.Epsilon.ToString(), int.MaxValue, long.MaxValue, "string");

            if (!result)
            {
                return MFTestResults.Fail;
            }

            return (result == true) ? MFTestResults.Pass : MFTestResults.Fail;            
        }

        #endregion
        
        #region Remove Tests

        [TestMethod]
        public MFTestResults Test_1_RemoveTest_0()
        {

            bool result = false;

            //result = stringBuilder.Clear().ToString() == string.Empty;
            result = stringBuilder.Clear().ToString() == null;

            if (!result)
            {
                return MFTestResults.Fail;
            }
           
            string testString = "0123456789";

            stringBuilder.Append(testString);

            stringBuilder.Remove(0, 1);

            result = stringBuilder.ToString() == "123456789";

            if (!result)
            {
                return MFTestResults.Fail;
            }

            stringBuilder.Remove(stringBuilder.Length - 1, 1);

            result = stringBuilder.ToString() == "12345678";

            if (!result)
            {
                return MFTestResults.Fail;
            }

            return (result == true) ? MFTestResults.Pass : MFTestResults.Fail;
        }

        #endregion

        #region Insert Tests

        [TestMethod]
        public MFTestResults Test_2_InsertTest_0()
        {
            stringBuilder.Clear();

            bool result = false;

            string testString = "0000";

            stringBuilder.Append(testString);

            stringBuilder.Insert(0, "x", 2);

            result = stringBuilder.ToString() == "xx0000";

            if (!result) return MFTestResults.Fail;

            stringBuilder.Insert(stringBuilder.Length, "x", 2);

            result = stringBuilder.ToString() == "xx0000xx";

            return (result == false) ? MFTestResults.Fail : MFTestResults.Pass;

        }

        #endregion

        #region Replace Tests

        [TestMethod]
        public MFTestResults Test_3_ReplaceTest_0()
        {

            stringBuilder.Clear();

            bool result = false;

            string testString = "0000";

            stringBuilder.Append(testString);

            stringBuilder.Append("_");

            stringBuilder.Append(testString);

            stringBuilder.Replace(testString, "xx");

            result = stringBuilder.ToString() == "xx_xx";

            return (result == false) ? MFTestResults.Fail : MFTestResults.Pass;

        }

        [TestMethod]
        public MFTestResults Test_3_ReplaceTest_1()
        {

            stringBuilder.Clear();

            bool result = false;

            string testString = "BEGIN";

            //stringBuilder.Append("BEGIN_MID_END");

            stringBuilder.Append('B');
            stringBuilder.Append('E');
            stringBuilder.Append('G');
            stringBuilder.Append('I');
            stringBuilder.Append('N');
            stringBuilder.Append('_');
            stringBuilder.Append('M');
            stringBuilder.Append('I');
            stringBuilder.Append('D');
            stringBuilder.Append('_');
            stringBuilder.Append('E');
            stringBuilder.Append('N');
            stringBuilder.Append('D');

            stringBuilder.Replace(testString, "xx");

            result = stringBuilder.ToString() == "xx_MID_END";

            return (result == false) ? MFTestResults.Fail : MFTestResults.Pass;

        }

        [TestMethod]
        public MFTestResults Test_3_ReplaceTest_2()
        {

            bool result = false;

            string testString = "MID";

            stringBuilder.Replace(testString, "xx");

            result = stringBuilder.ToString() == "xx_xx_END";

            return (result == false) ? MFTestResults.Fail : MFTestResults.Pass;

        }

        [TestMethod]
        public MFTestResults Test_3_ReplaceTest_3()
        {

            bool result = false;

            string testString = "END";

            stringBuilder.Replace(testString, "xx");

            result = stringBuilder.ToString() == "xx_xx_xx";

            return (result == false) ? MFTestResults.Fail : MFTestResults.Pass;

        }

        [TestMethod]
        public MFTestResults Test_3_ReplaceTest_4()
        {
            string testString = "The quick br!wn d#g jumps #ver the lazy cat.";
            bool result = false;
            stringBuilder = new System.Text.StringBuilder(testString);
                        
            stringBuilder.Replace('#', '!', 15, 29);        // Some '#' -> '!'

            result = stringBuilder.ToString() == "The quick br!wn d!g jumps !ver the lazy cat.";
            if (!result) return MFTestResults.Fail;

            stringBuilder.Replace('!', 'o');                // All '!' -> 'o'
            result = stringBuilder.ToString() == "The quick brown dog jumps over the lazy cat.";
            if (!result) return MFTestResults.Fail;

            stringBuilder.Replace("cat", "dog");            // All "cat" -> "dog"
            result = stringBuilder.ToString() == "The quick brown dog jumps over the lazy dog.";
            if (!result) return MFTestResults.Fail;

            stringBuilder.Replace("dog", "fox", 15, 20);    // Some "dog" -> "fox"
            result = stringBuilder.ToString() == "The quick brown fox jumps over the lazy dog.";
            if (!result) return MFTestResults.Fail;

            return MFTestResults.Pass;
        }

        #endregion

        #region Capacity Tests

        [TestMethod]
        public MFTestResults Test_4_CapacityTest_0()
        {

            stringBuilder.Length = 0;

            stringBuilder.Capacity = 5;

            bool result = false;

            //result = stringBuilder.ToString() == string.Empty;
            result = stringBuilder.ToString() == null;

            if (!result)
            {
                return MFTestResults.Fail;
            }

            string testString = "0000";

            stringBuilder.Append(string.Empty);

            stringBuilder.Append(testString);

            stringBuilder.Append(string.Empty);

            //should allocate here

            stringBuilder.Append("_");
            stringBuilder.Append("_");

            //result is true if capacity is > 5

            result = stringBuilder.Capacity > 5;

            if (!result)
            {
                return MFTestResults.Fail;
            }          

            return (result == false) ? MFTestResults.Fail : MFTestResults.Pass;

        }

        #endregion
    }
}
