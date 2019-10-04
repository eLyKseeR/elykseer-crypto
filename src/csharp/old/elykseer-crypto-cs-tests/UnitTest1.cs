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
            const string m = "Hello World!";
            var h = lxr.Md5.hash(m);
            Assert.AreEqual("ed076287532e86365e841e92bfc50d8c", h.tohex());
        }
    }
}