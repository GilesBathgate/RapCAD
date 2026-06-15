Primitive* CGALPrimitive::taper(const CGAL::Scalar& amount)
{
	this->buildPrimitive();
	CGALExplorer explorer(this);
	QList<QList<CGALPolygon*>> baseFaces = explorer.getBaseFaces();
	if (baseFaces.isEmpty()) {
		return this;
	}

	QMap<int, CGAL::Vector3> movements;

	for (const auto& faces : baseFaces) {
		OnceOnly first;
		for (CGALPolygon* pg : faces) {
			CGAL::Polygon_2<CGAL::Kernel3> polygon;
			const auto& pgPoints = pg->getProjectedPoints();
			for (const auto& p : pgPoints) {
				polygon.push_back(p);
			}

			if (polygon.is_clockwise_oriented()) {
				polygon.reverse_orientation();
			}

			bool isHole = !first();
			bool exterior = isHole ? (amount < 0.0) : (amount > 0.0);
			CGAL::Scalar abs_amount = CGAL::abs(amount);

			auto ss = (exterior) ?
				CGAL::create_exterior_straight_skeleton_2(abs_amount, polygon, CGAL::Kernel3()) :
				CGAL::create_interior_straight_skeleton_2(polygon, CGAL::Kernel3());

			if (!ss) continue;

			using Ss = CGAL::Straight_skeleton_2<CGAL::Kernel3>;
			using Traits = CGAL::Polygon_offset_builder_traits_2<CGAL::Kernel3>;
			CGAL::Polygon_offset_builder_2<Ss, Traits, CGAL::Polygon_2<CGAL::Kernel3>> ob(*ss);

			const auto& indexes = pg->getIndexes();
			// Map projected point to index
			QMap<CGAL::Point2, int> pointToIndex;
			for (int j = 0; j < pgPoints.size(); ++j) {
				pointToIndex[pgPoints[j]] = indexes[j];
			}

			for (auto it = ss->halfedges_begin(); it != ss->halfedges_end(); ++it) {
				if (it->is_bisector() && it->opposite()->vertex()->is_contour()) {
					auto cv = it->opposite()->vertex();
					if (pointToIndex.contains(cv->point())) {
						auto opt_p = ob.Construct_offset_point(abs_amount, it);
						if (opt_p) {
							CGAL::Point2 p2 = *opt_p;
							CGAL::Vector3 dir(p2.x() - cv->point().x(), p2.y() - cv->point().y(), 0);
							movements[pointToIndex[cv->point()]] = dir;
						}
					}
				}
			}
		}
	}

	CGALTaperModifier n(movements);
	nefPolyhedron->delegate(n, false, false);

	return this;
}
