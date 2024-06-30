import unittest
from py3dengine.wfoprocessor import WfoProcessor
from py3dengine.wfostatement import WfoStatement


class ProcessorTests(unittest.TestCase):
    def test_process_material_lib_statement(self):
        processor = WfoProcessor()

        test_data = 'some_file.mtl'

        processor.process_statements([
            WfoStatement(WfoStatement.Types.MATERIAL_LIB, test_data, 1, 1)
        ])

        self.assertEqual(
            (test_data,),
            processor.get_material_files(),
            'Material Lib statements add file names to material files collection'
        )

