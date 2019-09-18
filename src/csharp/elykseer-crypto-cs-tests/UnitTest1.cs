using NUnit.Framework;

namespace Tests
{
    public class Tests
    {
        [SetUp]
        public void Setup()
        {
        }

        [Test]
        public void TestMd5()
        {
            Assert.AreEqual("asdfasdf", lxr.Md5.hash("some text"));
        }
    }
}